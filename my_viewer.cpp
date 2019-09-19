# include "my_viewer.h"
# include "sn_mynode.h"
# include <sigogl/ui_button.h>
# include <sigogl/ui_radio_button.h>
# include <sig/sn_primitive.h>
# include <sig/sn_transform.h>
# include <sig/sn_manipulator.h>

# include <sigogl/ws_run.h>
# include <sigogl/gl_tools.h>

MyViewer::MyViewer ( int x, int y, int w, int h, const char* l ) : WsViewer(x,y,w,h,l)
{
	_nbut = 0;
	_animating = false;
	//add_ui ();
	add_mynode (10);
	build_ui();
	build_scene();
}
/*
void MyViewer::add_ui ()
{
	UiPanel *p;
	UiManager* uim = WsWindow::uim();
	p = uim->add_panel ( "", UiPanel::HorizLeft );
	p->add ( new UiButton ( "Add", EvAdd ) );
	p->add ( mcolorbut=new UiCheckButton ( "Multicolor", EvMultiColor ) );
	p->add ( new UiButton ( "OpenGL", EvInfo ) );
	p->add ( new UiButton ( "Exit", EvExit ) );
}
*/

void MyViewer::add_mynode ( int n )
{
	SnMyNode* c = new SnMyNode();

	//float r = 0.15f; // position range
	//float R = 0.75f; // big R arbitrary
	//float inc = 0.1f;
	while (n-- > 0)
	{
		for (int i = 0; i < n; i++)
		{
			points[i];
		}
	c->color(GsColor::darkred);
	// Example how to print/debug your generated data:
	// gsout<<n<<": "<<c->color()<<gsnl;
	rootg()->add(c);

	c->touch();
	render();
	}
}

void MyViewer::build_ui()
{
	UiPanel* p, * sp;
	UiManager* uim = WsWindow::uim();
	p = uim->add_panel("", UiPanel::HorizLeft);
	p->add(new UiButton("View", sp = new UiPanel()));
	{	UiPanel* p = sp;
	p->add(_nbut = new UiCheckButton("Normals", EvNormals));
	}
	p->add(new UiButton("Animate", EvAnimate));
	p->add(new UiButton("Exit", EvExit)); p->top()->separate();
}

void MyViewer::add_model(SnShape* s, GsVec p)
{
	// This method demonstrates how to add some elements to our scene graph: lines,
	// and a shape, and all in a group under a SnManipulator.
	// Therefore we are also demonstrating the use of a manipulator to allow the user to
	// change the position of the object with the mouse. If you do not need mouse interaction,
	// you can just use a SnTransform to apply a transformation instead of a SnManipulator.
	// You would then add the transform as 1st element of the group, and set g->separator(true).
	// Your scene graph should always be carefully designed according to your application needs.

	SnManipulator* manip = new SnManipulator;
	GsMat m;
	m.translation(p);
	manip->initial_mat(m);

	SnGroup* g = new SnGroup;
	SnLines* l = new SnLines;
	SnTransform* t = new SnTransform;
	l->color(GsColor::orange);
	g->add(s);
	g->add(l);
	g->add(t);
	manip->child(g);

	t->get().translation(p);

	manip->visible(false); // call this to turn off mouse interaction

	rootg()->add(manip);
}

void MyViewer::build_scene()
{
	SnPrimitive* p;
	SnGroup* g = new SnGroup;
	

	p = new SnPrimitive(GsPrimitive::Cylinder, 0.25, 0.25, 2.5f); // hand 1
	p->prim().material.diffuse = GsColor::red;
	// p->prim().orientation = GsQuat(GsVec::i, (float)GS_PIDIV2); 
	// rotate 90 degrees
	
	add_model(p, GsVec(0, 2.5f, 0.5f));

	// SnTransform* tr = (SnTransform*)((SnGroup*)((SnManipulator*)rootg()->get(0))->child())->get(2);
	// gsout << tr;

	p = new SnPrimitive(GsPrimitive::Cylinder, 0.5, 0, 0.5); // hand 1's arrow
	p->prim().material.diffuse = GsColor::red;

	add_model(p, GsVec(0, 5.5f, 0.5f));

	p = new SnPrimitive(GsPrimitive::Cylinder, 0.25, 0.25, 1); // hand 2
	p->prim().material.diffuse = GsColor::orange;

	add_model(p, GsVec(0, 1, 0.5f));

	p = new SnPrimitive(GsPrimitive::Cylinder, 0.5, 0, 0.5); // hand 2's arrow
	p->prim().material.diffuse = GsColor::orange;

	add_model(p, GsVec(0, 2.5f, 0.5f));

	/* extra stuff
	p = new SnPrimitive(GsPrimitive::Box,1,3,1);
	p = new SnPrimitive(GsPrimitive::Sphere,2);
	p = new SnPrimitive(GsPrimitive::Capsule,1,1,3);
	p = new SnPrimitive(GsPrimitive::Ellipsoid,2.0,0.5);
}

// Below is an example of how to control the main loop of an animation:
void MyViewer::run_animation()
{
	if (_animating) return; // avoid recursive calls
	_animating = true;

	int ind = gs_random(0, rootg()->size() - 1); // pick one child
	SnManipulator* manip = rootg()->get<SnManipulator>(ind); // access one of the manipulators
	GsMat m = manip->mat();

	double frdt = 1.0 / 30.0; // delta time to reach given number of frames per second
	double v = 4; // target velocity is 1 unit per second
	double t = 0, lt = 0, t0 = gs_time();
	do // run for a while:
	{
		while (t - lt < frdt) { ws_check(); t = gs_time() - t0; } // wait until it is time for next frame
		double yinc = (t - lt) * v;
		if (t > 2) yinc = -yinc; // after 2 secs: go down
		lt = t;
		m.e24 += (float)yinc;
		if (m.e24 < 0) m.e24 = 0; // make sure it does not go below 0
		manip->initial_mat(m);
		render(); // notify it needs redraw
		ws_check(); // redraw now
	} while (m.e24 > 0);
	_animating = false;
}

void MyViewer::show_normals(bool view)
{
	// Note that primitives are only converted to meshes in GsModel
	// at the first draw call.
	GsArray<GsVec> fn;
	SnGroup* r = (SnGroup*)root();
	for (int k = 0; k < r->size(); k++)
	{
		SnManipulator* manip = r->get<SnManipulator>(k);
		SnShape* s = manip->child<SnGroup>()->get<SnShape>(0);
		SnLines* l = manip->child<SnGroup>()->get<SnLines>(1);
		if (!view) { l->visible(false); continue; }
		l->visible(true);
		if (!l->empty()) continue; // build only once
		l->init();
		if (s->instance_name() == SnPrimitive::class_name)
		{
			GsModel& m = *((SnModel*)s)->model();
			m.get_normals_per_face(fn);
			const GsVec* n = fn.pt();
			float f = 0.33f;
			for (int i = 0; i < m.F.size(); i++)
			{
				const GsVec& a = m.V[m.F[i].a]; l->push(a, a + (*n++) * f);
				const GsVec& b = m.V[m.F[i].b]; l->push(b, b + (*n++) * f);
				const GsVec& c = m.V[m.F[i].c]; l->push(c, c + (*n++) * f);
			}
		}
	}
}


int MyViewer::handle_keyboard ( const GsEvent &e )
{
	int ret = WsViewer::handle_keyboard ( e ); // 1st let system check events
	if ( ret ) return ret;

	switch ( e.key )
	{	case GsEvent::KeyEsc : gs_exit(); return 1;
		case GsEvent::KeyLeft: gsout<<"Left\n"; return 1;
		default: gsout<<"Key pressed: "<<e.key<<gsnl;
		
		// case 'q': 
	}

	return 0;
}
/*
int MyViewer::uievent ( int e )
{
	switch ( e )
	{	case EvAdd: add_mynode(1); return 1;

		case EvInfo:
		{	if ( output().len()>0 ) { output(""); return 1; }
			output_pos(0,30);
			activate_ogl_context(); // we need an active context
			GsOutput o; o.init(output()); gl_print_info(&o); // print info to viewer
			return 1;
		}

		case EvExit: gs_exit();
	}
	return WsViewer::uievent(e);
}
*/
