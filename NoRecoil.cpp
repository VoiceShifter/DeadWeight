# include "NoRecoil.hpp"
//========================================================================
std::atomic<bool> StopThread(false), StopCheckerThread(false);
std::thread MovingMouseThread;
std::thread Checker;

# define ConverseX 34.1505992
# define ConverseY 60.7367933
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


void DetectMouseClick(unsigned int Speed)
{
      unsigned int xCoordinate{}, yCoordinate{};      
      INPUT Mouse_Input{};
	while (1)
	{
		if (GetKeyState(VK_LBUTTON) & 0x100)            
		{

                  GetCursorPos(&CursorCoodinates);
                  xCoordinate = int(65535.0 / 1919 * CursorCoodinates.x); //+ 170
                  yCoordinate = int(65535.0 / 1079 * int(CursorCoodinates.y) + Speed * ConverseY); //- 1
                  //mouse_event(MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, xCoordinate, yCoordinate, 0, 0);
                  Mouse_Input.type = INPUT_MOUSE;
                  Mouse_Input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
                  Mouse_Input.mi.dx = xCoordinate;
                  Mouse_Input.mi.dy = yCoordinate;
                  SendInput(1, &Mouse_Input, sizeof(Mouse_Input));

                  
		}
            

            if (StopThread == true)
            {
                  return;
            }

		std::this_thread::sleep_for(std::chrono::milliseconds(25));		
	}
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
                        std::this_thread::sleep_for(std::chrono::milliseconds(500));
                  }
                  else
                  {                        
                        MovingMouseThread = std::thread(DetectMouseClick, Speed);
                        std::this_thread::sleep_for(std::chrono::milliseconds(500));
                  }
            }
            
            if (StopCheckerThread)
            {
                  return;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(25));
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