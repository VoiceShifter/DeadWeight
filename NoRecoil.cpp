# include "NoRecoil.hpp"
//========================================================================
std::atomic<bool> StopThread(false), StopCheckerThread(false);
std::thread MovingMouseThread;
std::thread Checker;

# define ConverseX 34.1333333
# define ConverseY 60.6814814
POINT CursorCoodinates{};
bool SetCursorBool = false;


//========================================================================

NoRecoil::NoRecoil(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	QObject::connect(ui.ConfirmButton, SIGNAL(clicked()), this, SLOT(ChangeSpeedButtonClicked()));
      

}

NoRecoil::~NoRecoil()
{

	if (MovingMouseThread.joinable())
	{
		StopThread = true;
		MovingMouseThread.join();
	}
      if (Checker.joinable())
      {
            StopCheckerThread = true;
            Checker.join();
      }

      
}

inline void MoveMouse(unsigned int Speed)
{
	
	

	SetCursorBool = SetCursorPos(960, 540+(1.5 + Speed));
      
}

void DetectMouseClick(unsigned int Speed)
{
      unsigned int Offset{ 0 };
      unsigned int xCoordinate{}, yCoordinate{};      
      
	while (1)
	{
            //if (true)
		if (GetKeyState(VK_LBUTTON) & 0x100)            
		{
                  GetCursorPos(&CursorCoodinates);
                  xCoordinate = int(65535.0 / 1919 * CursorCoodinates.x); //+ 170
                  yCoordinate = int(65535.0 / 1079 * int(CursorCoodinates.y) + Speed * ConverseY); //- 1
                  mouse_event(MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, xCoordinate, yCoordinate, 0, 0);
                  //MoveMouse(Offset);
                  //Offset += Speed;
		}
            else
            {
                  Offset = 0;
            }

		std::this_thread::sleep_for(std::chrono::milliseconds(25));		
		if (GetKeyState(VK_INSERT) < 0 || StopThread)
		{
			return;
		}
	}
}
void NoRecoil::DetectSpeedChange(unsigned int OriginalSpeed)	  //useless in the end, since i dont want to use another thread to overload cpu
{
	double Speed = ui.SpeedField->text().toDouble();
	if (Speed != OriginalSpeed)
	{
		StopThread = true;
		MovingMouseThread.join();
		StopThread = false;
		MovingMouseThread = std::thread(DetectMouseClick, Speed);
	}
	else
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

}

void NoRecoil::ChangeSpeed(unsigned int Speed)
{
	if (MovingMouseThread.joinable())
	{
		StopThread = true;
		MovingMouseThread.join();
		StopThread = false;
	}
	MovingMouseThread = std::thread(DetectMouseClick, Speed);      
}

void CheckerFunction(unsigned int Speed)
{
      
      while (1)
      {
            if (GetKeyState(VK_INSERT) < 0 || GetKeyState(VK_INSERT) & 0x100)
            {
                  if (MovingMouseThread.joinable())
                  {
                        StopThread = true;
                        MovingMouseThread.join();
                        StopThread = false;
                  }
                  else
                  {                        
                        MovingMouseThread = std::thread(DetectMouseClick, Speed);
                  }
            }
            
            if (StopCheckerThread)
            {
                  return;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
      }
      
};

unsigned int NoRecoil::ChangeSpeedButtonClicked()
{
	double Speed = ui.SpeedField->text().toDouble();
      if (Checker.joinable())
      {
            StopCheckerThread = 1;
            Checker.join();
            StopCheckerThread = 0;
      }
      Checker = std::thread(CheckerFunction, Speed);
	return 0;
}