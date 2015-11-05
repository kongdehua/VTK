#ifndef _TIMEWIDGET_H_
#define _TIMEWIDGET_H_

#include "vtkAbstractWidget.h"

#include <vector>

class vtkConeSource;
class vtkPolyDataMapper;
class vtkPoints;
class vtkCellArray;
class vtkFloatArray;
class vtkPolyData;
class vtkPolyDataMapper;
class vtkActor;

class vtkHandleWidget;
class vtkTimerWidgetCallback;
class vtkPointHandleRepresentation2D;
class vtkPointHandleRepresentation3D;
class vtkTimerCallback;
class vtkButtonWidget;

class TimerWidget : public vtkAbstractWidget
{
public:
	static TimerWidget *New();

	vtkTypeMacro(TimerWidget, vtkAbstractWidget);
	void PrintSelf(ostream &os, vtkIndent indent);

	virtual void SetEnabled(int );

	virtual void CreateDefaultRepresentation();

public:
	void CreateTimer();
	void SetVisualData(std::vector<std::vector<double> > gFile);
	void Update();

protected:
	TimerWidget();
	~TimerWidget();

	static void ButtonDown(vtkAbstractWidget *);
	static void ButtonUp(vtkAbstractWidget *);
	static void MouseMove(vtkAbstractWidget *);

protected:
	vtkHandleWidget *PointWidget;
	vtkTimerWidgetCallback *timerWidgetCallback;
	vtkTimerCallback *timerCallback;
	//vtkPointHandleRepresentation2D *m_repPoint;
	vtkPointHandleRepresentation3D *m_repPoint;

	vtkButtonWidget *m_btnStart;
	vtkButtonWidget *m_btnEnd;

private:
	TimerWidget(const TimerWidget&);
	void operator=(const TimerWidget&);

private:
	void initButtonWidget();

private:
	vtkConeSource *m_coneSource; 
	vtkPolyDataMapper *m_coneMapper; 
	vtkActor *m_coneActor; 

	vtkPoints *points;
	vtkCellArray *triangles ;
	vtkFloatArray *colors ;
	vtkPolyData *polydata ;
	vtkPolyDataMapper *mapper ;
	std::vector<std::vector<double> > g_file;
	int m_nCurIndex;

	vtkActor *actorResult;
};

#endif
