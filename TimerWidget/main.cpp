#include <iostream>

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

#include <QFile>
#include <QDebug>
#include <QStringList>
#include <QString>
#include <QTextStream>

#include <vector>

#include "TimerWidget.h"


std::vector<std::vector<double> > g_file;
void readFile( const std::string filename, std::vector<std::vector<double> > &vecData)
{
	QFile file(filename.c_str());	
	std::cout << filename << std::endl;
	if (!file.open(QIODevice::ReadOnly))
	{
		return ;
	}

	QTextStream in(&file);

	// Get frequency info
	QString line = in.readLine();   // Frequency: 
	line = in.readLine();   // 1.0000000000000000e+011	1.0000000000000000e+011	1
	line = in.readLine();   // 1.0000000000000000e+011
	// Get iSrfIdx
	in.readLine();   // -1
	// Get RCCounter
	in.readLine();   // RCCounter:
  in.readLine();   // 2 [ -1, 0 ]
	// Get Data
  in.readLine();   // Data:
	in.readLine();   // 2
	line = in.readLine();   // 1 17583
	QStringList fields = line.split('\t');
	int nStart = fields[0].toInt();
	int nEnd   = fields[1].toInt();
	std::cout << "Start End: " << nStart << "  " << nEnd << std::endl;

	//g_file.resize(nEnd);
	g_file.clear();

	std::vector<double> vec;
	// get All Data
	while(!in.atEnd()) {
		vec.clear();
		QString line = in.readLine();
		QStringList fields = line.split(',');

		foreach(QString field, fields)
		{
			vec.push_back(field.toDouble());
		}

		//std::cout << "vec line is " << vec.size() << std::endl;
		g_file.push_back(vec);
	}
	//std::cout << "file line is " << g_file.size() << std::endl;
}

int main()
{
	readFile("/home/kdh/VTKData/1.cur", g_file);
	//readFile("/home/kdh/VTKData/Fs100Fe100GHzFN1IdealElectricDipole_Pos(0,0,10)_Dir(0,0,1)SubID(-1,0)_CurrentsObs0DeltaObsFs100Fe100GHzFN1SrfID-1.cur", g_file);
	vtkRenderWindow *win = vtkRenderWindow::New();
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(win);

	vtkRenderer *ren = vtkRenderer::New();
	ren->SetBackground(1.0, 0.0, 0.0);
	ren->GetActiveCamera();
	win->AddRenderer(ren);

	TimerWidget *wdg = TimerWidget::New();
	wdg->SetInteractor(iren);
	wdg->SetVisualData(g_file);

	iren->Initialize();
	wdg->On();
	iren->Start();
	return 0;
};

