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


void DetectMouseClick(unsigned int Speed)
{
      unsigned int xCoordinate{}, yCoordinate{};      
      INPUT Mouse_Input{};
	while (1)
	{
		if (GetKeyState(VK_LBUTTON) & 0x100)
		{      
                  mouse_event(MOUSEEVENTF_MOVE, 0, 0+Speed, 0, 0); 
		}
            
            if (MouseThreadStopper == true)
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
                        MouseThreadStopper = true;
                        MovingMouseThread.join();
                        MouseThreadStopper = false;
                        std::this_thread::sleep_for(std::chrono::milliseconds(500));
                  }
                  else
                  {                        
                        MouseThreadStopper = false;
                        MovingMouseThread = std::thread(DetectMouseClick, Speed);
                        std::this_thread::sleep_for(std::chrono::milliseconds(500));
                  }
            }
            
            if (StopCheckerThread)
            {
                  MouseThreadStopper = true;
                  MovingMouseThread.join();
                  MouseThreadStopper = false;
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