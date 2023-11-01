# include "NoRecoil.hpp"
//========================================================================
std::atomic<bool> MouseThreadStopper(false), StopCheckerThread(false);
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
		MouseThreadStopper = true;
		MovingMouseThread.join();
	}
      if (Checker.joinable())
      {
            StopCheckerThread = true;
            Checker.join();
      }

      
}


void DetectMouseClick(unsigned int Speed, signed int ySpeed)
{

      for(;;)
	{
		if (GetKeyState(VK_LBUTTON) & 0x100)
		{      
                  mouse_event(MOUSEEVENTF_MOVE, 0+ySpeed, 0+Speed, 0, 0); 
		}
            
            if (MouseThreadStopper == true)
            {
                  return;
            }

		std::this_thread::sleep_for(std::chrono::milliseconds(25));		
	}
}


void CheckerFunction(unsigned int Speed, signed int ySpeed = 0)
{
      
      for(;;)
      {
            if (GetKeyState(0x31) < 0 || GetKeyState(0x31) & 0x100)
            {
                  if (MovingMouseThread.joinable())
                  {
                        MouseThreadStopper = true;
                        MovingMouseThread.join();
                        MouseThreadStopper = false;
                        std::this_thread::sleep_for(std::chrono::milliseconds(500));
                  }
                  else
                  {                        
                        MouseThreadStopper = false;
                        MovingMouseThread = std::thread(&DetectMouseClick, Speed, ySpeed);
                        std::this_thread::sleep_for(std::chrono::milliseconds(500));
                  }
            }
            else if (GetKeyState(0x32) < 0 || GetKeyState(0x33) < 0 || GetKeyState(0x34) < 0 || GetKeyState(0x35) < 0)
            {
                  if (MovingMouseThread.joinable())
                  {
                        MouseThreadStopper = true;
                        MovingMouseThread.join();
                        MouseThreadStopper = false;
                        std::this_thread::sleep_for(std::chrono::milliseconds(500));
                  }
            }
          
            if (StopCheckerThread == true)
            {
                  
                  if (MovingMouseThread.joinable())
                  {
                        MouseThreadStopper = true;
                        MovingMouseThread.join();
                        MouseThreadStopper = false;                     
                  }
                  return;                 
               
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(25));
      }
      
};

unsigned int NoRecoil::ChangeSpeedButtonClicked()
{
	double Speed = ui.SpeedField->text().toDouble(), ySpeed = ui.ySpeedField->text().toDouble();
      if (Checker.joinable())
      {
            StopCheckerThread = true;
            Checker.join();
            StopCheckerThread = false;
      }
      Checker = std::thread(&CheckerFunction, Speed, ySpeed);
	return 0;
}

