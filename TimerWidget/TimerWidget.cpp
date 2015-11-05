#include "TimerWidget.h"

#include <vtkCommand.h>
#include <vtkWidgetEvent.h>
#include <vtkObjectFactory.h>
#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

#include <vtkHandleWidget.h>
#include <vtkWidgetCallbackMapper.h>
#include <vtkWidgetEventTranslator.h>
#include <vtkCallbackCommand.h>

#include <vtkPointHandleRepresentation2D.h>
#include <vtkPointHandleRepresentation3D.h>
#include <vtkButtonWidget.h>
#include <vtkImageData.h>
#include <vtkTexturedButtonRepresentation2D.h>
#include <vtkCellArray.h>
#include <vtkTriangle.h>
#include <vtkProperty.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>

vtkStandardNewMacro(TimerWidget);

class vtkButtonStartCallback : public vtkCommand
{
public:
	static vtkButtonStartCallback *New()
	{ return new vtkButtonStartCallback(); }

	virtual void Execute(vtkObject* obj, unsigned long eventId, void*)
	{
		TimerWidget *wdg = reinterpret_cast<TimerWidget*>(obj);

		if (NULL == wdg)
			std::cout << "error" << std::endl;
		//wdg->CreateTimer();
		if (1 == m_btnStart->GetSliderRepresentation()->GetState())
		{
			m_nTimer = wdg->GetInteractor()->CreateRepeatingTimer(100);
		}
		else
		{
			wdg->GetInteractor()->DestroyTimer(m_nTimer);
			m_nTimer = -1;
		}

		//wdg->GetInteractor()->AddObserver(vtkCommand::TimerEvent, timerCallback);
		//wdg->GetInteractor()->CreateRepeatingTimer(100);
	}

public:
 	void SetButtonWidget(vtkButtonWidget *wdg)
	{
		m_btnStart = wdg;
	}

private:
	int m_nTimer;
	vtkButtonWidget *m_btnStart;
};

class vtkTimerWidgetCallback : public vtkCommand
{
public:
	static vtkTimerWidgetCallback *New()
	{ return new vtkTimerWidgetCallback(); }

	virtual void Execute(vtkObject*, unsigned long eventId, void*)
	{
		switch (eventId)
		{
		default:
			break;
		}
	}

private:
};

class vtkTimerCallback : public vtkCommand
{
public:
	static vtkTimerCallback *New()
	{ return new vtkTimerCallback; }

	virtual void Execute(vtkObject *obj, 
			unsigned long eventId, void *)
	{
		if (vtkCommand::TimerEvent == eventId)
		{
			TimerWidget *wdg = reinterpret_cast<TimerWidget*>(obj);
			wdg->Update();
		}
	}
};

TimerWidget::TimerWidget()
	: actorResult(NULL)
  , m_nCurIndex(-1)
{
	m_coneSource = vtkConeSource::New();
	m_coneMapper = vtkPolyDataMapper::New();
	m_coneMapper->SetInputConnection(m_coneSource->GetOutputPort());
	m_coneActor = vtkActor::New();
	m_coneActor->SetMapper(m_coneMapper);

	PointWidget = vtkHandleWidget::New();
	PointWidget->SetParent(this);

	timerWidgetCallback = vtkTimerWidgetCallback::New();
	/*
	PointWidget->AddObserver(
			vtkCommand::StartInteractionEvent,
			timerWidgetCallback,
			this->Priority);
	PointWidget->AddObserver(
			vtkCommand::InteractionEvent,
			timerWidgetCallback,
			this->Priority);
	PointWidget->AddObserver(
			vtkCommand::EndInteractionEvent,
			timerWidgetCallback,
			this->Priority);
			*/

	timerCallback = vtkTimerCallback::New();
	
	// These are the event callbacks supported by this widget
	/*
	CallbackMapper->SetCallbackMethod(
			vtkCommand::LeftButtonPressEvent,
			vtkWidgetEvent::Select,
			this,
			TimerWidget::ButtonDown);
	CallbackMapper->SetCallbackMethod(
			vtkCommand::LeftButtonReleaseEvent,
			vtkWidgetEvent::EndSelect,
			this,
			TimerWidget::ButtonUp);
	CallbackMapper->SetCallbackMethod(
			vtkCommand::MouseMoveEvent,
			vtkWidgetEvent::Move,
			this,
			TimerWidget::MouseMove);
			*/

	points = vtkPoints::New();
	triangles = vtkCellArray::New();
	colors = vtkFloatArray::New();

	polydata = vtkPolyData::New();
	polydata->SetPoints(points);
	polydata->GetPointData()->SetScalars(colors);
	polydata->SetPolys(triangles);

	mapper = vtkPolyDataMapper::New();
	mapper->SetInputData(polydata);

	actorResult = vtkActor::New();
	actorResult->SetMapper(mapper);
	actorResult->GetProperty()->SetRepresentationToSurface();
}

TimerWidget::~TimerWidget()
{
}

void TimerWidget::CreateDefaultRepresentation()
{
}

void TimerWidget::SetEnabled(int enabling)
{
	int X = this->Interactor->GetEventPosition()[0];
	int Y = this->Interactor->GetEventPosition()[1];

	if (enabling)
	{

		if (!this->CurrentRenderer)
		{
			this->SetCurrentRenderer(this->Interactor->FindPokedRenderer(X,Y));
			if (!this->CurrentRenderer) return;

			PointWidget->SetCurrentRenderer(this->CurrentRenderer);
		}

		this->Enabled = 1;

		//this->CurrentRenderer->AddActor(m_coneActor);

		this->EventTranslator->AddEventsToInteractor(
				this->Interactor,
				this->EventCallbackCommand,
				this->Priority);

	this->Interactor->AddObserver(vtkCommand::TimerEvent, timerCallback);


		if (PointWidget)
		{
			std::cout << "Point" << std::endl;
			//m_repPoint = vtkPointHandleRepresentation2D::New();
			m_repPoint = vtkPointHandleRepresentation3D::New();
			PointWidget->SetRepresentation(m_repPoint);
			m_repPoint->SetRenderer(this->CurrentRenderer);
			PointWidget->SetInteractor(this->Interactor);
			PointWidget->SetCurrentRenderer(this->CurrentRenderer);
			//PointWidget->SetEnabled(1);
		}

		initButtonWidget();

		/*
		this->Interactor->AddObserver(vtkCommand::TimerEvent, timerCallback);
		this->Interactor->CreateRepeatingTimer(100);
		*/

		if (actorResult)
		{
			this->CurrentRenderer->AddActor(actorResult);
		}

		CurrentRenderer->ResetCamera();
	}
	else
	{
		this->Interactor->RemoveObserver(
				this->EventCallbackCommand);

		PointWidget->SetEnabled(0);
	}
}

void TimerWidget::ButtonDown(vtkAbstractWidget *w)
{
	TimerWidget *self = reinterpret_cast<TimerWidget*>(w);

	/*
	self->GrabFocus(self->EventCallbackCommand);
	self->EventCallbackCommand->SetAbortFlag(1);

	int X = self->Interactor->GetEventPosition()[0];
	int Y = self->Interactor->GetEventPosition()[1];

	double e[3];
	e[0] = static_cast<double> (X);
	e[1] = static_cast<double> (Y);
	e[2] = 0.0;
	self->m_repPoint->SetDisplayPosition(e);
	double p[3];
	self->m_repPoint->GetWorldPosition(p);
	self->m_repPoint->SetWorldPosition(p);
	*/
	
	self->Render();
	//std::cout << "ButtonDown" << std::endl;
}

void TimerWidget::ButtonUp(vtkAbstractWidget *w)
{
	TimerWidget *self = reinterpret_cast<TimerWidget*>(w);

	//self->ReleaseFocus();
	//std::cout << "ButtonUp" << std::endl;
}

void TimerWidget::MouseMove(vtkAbstractWidget *w)
{
	//std::cout << "MouseMove" << std::endl;
	TimerWidget *self = reinterpret_cast<TimerWidget*>(w);

	/*
	self->GrabFocus(self->EventCallbackCommand);
	self->EventCallbackCommand->SetAbortFlag(1);

	int X = self->Interactor->GetEventPosition()[0];
	int Y = self->Interactor->GetEventPosition()[1];

	double e[3];
	e[0] = static_cast<double> (X);
	e[1] = static_cast<double> (Y);
	e[2] = 0.0;
	self->m_repPoint->SetDisplayPosition(e);
	double p[3];
	self->m_repPoint->GetWorldPosition(p);
	self->m_repPoint->SetWorldPosition(p);
	*/
	
	//self->Render();
}


void TimerWidget::PrintSelf(ostream &os, vtkIndent indent)
{
}

void CreateImage(vtkImageData *image,
                 unsigned char* color1,
                 unsigned char* color2)
{
  // Specify the size of the image data
  image->SetDimensions(10,10,1);
#if VTK_MAJOR_VERSION <= 5
  image->SetNumberOfScalarComponents(3);
  image->SetScalarTypeToUnsignedChar();
#else
  image->AllocateScalars(VTK_UNSIGNED_CHAR,3);
#endif
  int* dims = image->GetDimensions();
 
  // Fill the image with
  for (int y = 0; y < dims[1]; y++)
    {
    for (int x = 0; x < dims[0]; x++)
      {
      unsigned char* pixel =
        static_cast<unsigned char*>(image->GetScalarPointer(x,y,0));
      if(x<5)
        {
        pixel[0] = color1[0];
        pixel[1] = color1[1];
        pixel[2] = color1[2];
        }
      else
        {
        pixel[0] = color2[0];
        pixel[1] = color2[1];
        pixel[2] = color2[2];
        }
      }
    }
}

void TimerWidget::initButtonWidget()
{
	vtkImageData *image1 = vtkImageData::New();
	vtkImageData *image2 = vtkImageData::New();
	unsigned char banana[3] = {227, 207, 87};
	unsigned char tomato[3] = {255, 99, 87};
	CreateImage(image1, banana, tomato);
	CreateImage(image2, tomato, banana);
	
	vtkTexturedButtonRepresentation2D *rep = vtkTexturedButtonRepresentation2D::New();
	rep->SetNumberOfStates(2);
	rep->SetButtonTexture(0, image1);
	rep->SetButtonTexture(1, image2);
	
	m_btnStart = vtkButtonWidget::New();
	m_btnStart->SetPriority(4.0);
	m_btnStart->SetInteractor(this->Interactor);
	m_btnStart->SetRepresentation(rep);

	vtkButtonStartCallback *callback = vtkButtonStartCallback::New();
	callback->SetButtonWidget(m_btnStart);

	m_btnStart->AddObserver(vtkCommand::StateChangedEvent, callback);
	m_btnStart->On();
}


void TimerWidget::CreateTimer()
{
	this->Interactor->CreateRepeatingTimer(100);
}


/*
 * show 
 */
void TimerWidget::SetVisualData(
		std::vector<std::vector<double> > gFile)
{

	g_file = gFile;
	// one point per line
	// one triangle per three point
	double point[3] = {0};
	for (int nIndex = 0; nIndex < gFile.size()/3; nIndex++)
	{
		point[0] = gFile[3*nIndex+0][0];
		point[1] = gFile[3*nIndex+0][1];
		point[2] = gFile[3*nIndex+0][2];
		points->InsertNextPoint(point);

		point[0] = gFile[3*nIndex+1][0];
		point[1] = gFile[3*nIndex+1][1];
		point[2] = gFile[3*nIndex+1][2];
		points->InsertNextPoint(point);

		point[0] = gFile[3*nIndex+2][0];
		point[1] = gFile[3*nIndex+2][1];
		point[2] = gFile[3*nIndex+2][2];
		points->InsertNextPoint(point);

		vtkTriangle *tri = vtkTriangle::New();
		tri->GetPointIds()->SetId(0, 3*nIndex+0);
		tri->GetPointIds()->SetId(1, 3*nIndex+1);
		tri->GetPointIds()->SetId(2, 3*nIndex+2);
		triangles->InsertNextCell(tri);

		colors->InsertNextValue(gFile[3*nIndex+0][4]);
		colors->InsertNextValue(gFile[3*nIndex+1][4]);
		colors->InsertNextValue(gFile[3*nIndex+2][4]);

		/*
		vtkIdType id[3] = {0};
		id[0] = 3*nIndex+0;
		id[1] = 3*nIndex+1;
		id[2] = 3*nIndex+2;
		triangles->InsertNextCell(id);
		*/
	}
	points->Modified();
	triangles->Modified();
	colors->Modified();

	
	//point->InserNextPoint();
}

void TimerWidget::Update()
{
	std::cout << g_file.size() << std::endl;

	/*
	m_nCurIndex = (m_nCurIndex+1)%4;

	// one point per line
	// one triangle per three point
	double point[3] = {0};
	colors = vtkFloatArray::New();
	for (int nIndex = 0; nIndex < g_file.size()/3; nIndex++)
	{
		colors->InsertNextValue(g_file[3*nIndex+0][m_nCurIndex*6+4]);
		colors->InsertNextValue(g_file[3*nIndex+1][m_nCurIndex*6+4]);
		colors->InsertNextValue(g_file[3*nIndex+2][m_nCurIndex*6+4]);

	}
	polydata->GetPointData()->SetScalars(colors);
	colors->Modified();

	this->Interactor->Render();
	*/
	//point->InserNextPoint();
}


