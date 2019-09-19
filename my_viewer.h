# pragma once

# include <sigogl/ui_button.h>
# include <sigogl/ui_check_button.h>
# include <sig/sn_poly_editor.h>
# include <sig/sn_lines2.h>

# include <sigogl/ui_button.h>
# include <sigogl/ws_viewer.h>

// Viewer for this example:
class MyViewer : public WsViewer
{  protected :
	// enum MenuEv { EvAdd, EvMultiColor, EvInfo, EvExit };
	enum MenuEv { EvNormals, EvAnimate, EvExit };
	UiCheckButton* mcolorbut;
	UiCheckButton* _nbut;
	bool _animating;
   public :
	   GsArray<GsPnt> points;
	MyViewer ( int x, int y, int w, int h, const char* l );
	//void add_ui ();
	void add_mynode ( int n );
	void build_ui();
	void add_model(SnShape* s, GsVec p);
	void build_scene();
	void show_normals(bool view);
	void run_animation();
	virtual int handle_keyboard ( const GsEvent &e ) override;
	//virtual int uievent ( int e ) override;
};
