# include "NoRecoil.hpp"

std::atomic<bool> StopThread(false);
std::thread MovingMouseThread;
NoRecoil::NoRecoil(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	QObject::connect(ui.ConfirmButton, SIGNAL(clicked()), this, SLOT(ChangeSpeedButtonClicked()));
}

NoRecoil::~NoRecoil()
{}

inline void MoveMouse(double Speed)
{
	SetCursorPos(0, -1 * Speed);
}

void DetectMouseClick(double Speed)
{
	if (GetKeyState(VK_LBUTTON) < 0)
	{
		MoveMouse(Speed);
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	if (GetKeyState(VK_F1) < 0 || StopThread)
	{
		return;
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
	StopThread = true;
	MovingMouseThread.join();
	StopThread = false;
	MovingMouseThread = std::thread(DetectMouseClick, Speed);
}
unsigned int NoRecoil::ChangeSpeedButtonClicked()
{
	double Speed = ui.SpeedField->text().toDouble();
	
	
	//start thread


}