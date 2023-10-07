# include "NoRecoil.hpp"

std::atomic<bool> StopThread(false), StopCheckerThread(false);
std::thread MovingMouseThread;
std::thread Checker;

LPPOINT CursorCoodinates{};
bool SetCursorBool = false;


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
      
}

inline void MoveMouse(double Speed)
{
	
	//GetCursorPos(CursorCoodinates);

	SetCursorBool = SetCursorPos(960, 540+(1.5 + Speed));
      
}

void DetectMouseClick(double Speed)
{

      unsigned int Offset{0};
	while (1)
	{
		if (GetKeyState(VK_LBUTTON) & 0x100)
		{
			MoveMouse(Offset);
                  Offset += Speed;
		}
            else
            {
                  Offset = 0;
            }

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		//SetCursorPos(0, -3 * Speed);
		if (GetKeyState(VK_F1) < 0 || StopThread)
		{
			return;
		}
	}
}
void NoRecoil::DetectSpeedChange(double OriginalSpeed)	  //useless in the end, since i dont want to use another thread to overload cpu
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

void NoRecoil::ChangeSpeed(double Speed)
{
	if (MovingMouseThread.joinable())
	{
		StopThread = true;
		MovingMouseThread.join();
		StopThread = false;
	}
	MovingMouseThread = std::thread(DetectMouseClick, Speed);      
}

void CheckerFunction(double Speed)
{
      
      while (1)
      {
            if (GetKeyState(VK_F1) < 0 || GetKeyState(VK_F1) & 0x100)
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
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
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