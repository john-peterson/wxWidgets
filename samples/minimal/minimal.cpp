/////////////////////////////////////////////////////////////////////////////
// Name:        minimal.cpp
// Purpose:     Minimal wxWidgets sample
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#ifdef __WXGTK__
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
//#include <gdk/gdkx11device-xi2.h>
//#include <gdk/gdkdeviceprivate.h>
#include <X11/Xlib.h>
#include <X11/extensions/XInput2.h>
#endif

// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------

// the application icon (under Windows and OS/2 it is in resources and even
// though we could still include the XPM here it would be unused)
#ifndef wxHAS_IMAGES_IN_RESOURCES
    #include "../sample.xpm"
#endif

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp
{
public:
    // override base class virtuals
    // ----------------------------

    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    virtual bool OnInit();
};

// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame
{
public:
    // ctor(s)
    MyFrame(const wxString& title);

    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

	void Grab(bool grab);

	void OnKeyDown(wxKeyEvent& event);
	void OnMouseDown(wxMouseEvent& event);

private:
    // any class wishing to process wxWidgets events must use this macro
    DECLARE_EVENT_TABLE()

	wxPanel *panel;
};

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // menu items
    Minimal_Quit = wxID_EXIT,

    // it is important for the id corresponding to the "About" command to have
    // this standard value as otherwise it won't be handled properly under Mac
    // (where it is special and put into the "Apple" menu)
    Minimal_About = wxID_ABOUT
};

// ----------------------------------------------------------------------------
// event tables and other macros for wxWidgets
// ----------------------------------------------------------------------------

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(Minimal_Quit,  MyFrame::OnQuit)
    EVT_MENU(Minimal_About, MyFrame::OnAbout)
END_EVENT_TABLE()

// Create a new application object: this macro will allow wxWidgets to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also implements the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
IMPLEMENT_APP(MyApp)

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
	//gdk_disable_multidevice();

    // call the base class initialization method, currently it only parses a
    // few common command-line options but it could be do more in the future
    if ( !wxApp::OnInit() )
        return false;

    // create the main application window
    MyFrame *frame = new MyFrame("Minimal wxWidgets App");

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    frame->Show(true);
	frame->SetFocus();

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned false here, the
    // application would exit immediately.
    return true;
}

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
MyFrame::MyFrame(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title)
{
	wxTheApp->Bind(wxEVT_KEY_DOWN, &MyFrame::OnKeyDown, this);
	wxTheApp->Bind(wxEVT_LEFT_DOWN, &MyFrame::OnMouseDown, this);

    // set the frame icon
    SetIcon(wxICON(sample));

#if wxUSE_MENUS
    // create a menu bar
    wxMenu *fileMenu = new wxMenu;

    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(Minimal_About, "&About\tF1", "Show about dialog");

    fileMenu->Append(Minimal_Quit, "E&xit\tAlt-X", "Quit this program");

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(helpMenu, "&Help");

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
    // create a status bar just for fun (by default with 1 pane only)
    CreateStatusBar(2);
    SetStatusText("Welcome to wxWidgets!");
#endif // wxUSE_STATUSBAR

	panel = new wxPanel(this);
}

void MyFrame::OnKeyDown(wxKeyEvent& event)
{
	if (event.GetKeyCode() == 'Q')
		Close();

	//wxMessageBox(wxString::Format("OnKeyDown"), "");
	//fprintf(stderr, "%d %d\n"
	//	, event.GetKeyCode()
	//	, event.GetKeyCode() == 'Q'
	//	);
	
	event.Skip();

	if (event.GetKeyCode() == 'G')
		Grab(true);
	else
		Grab(false);
}

void MyFrame::OnMouseDown(wxMouseEvent& event)
{
	//if (event)
		//Grab(true);
	//else
		//Grab(false);

	event.Skip();
}

#ifdef __WXGTK__
Display *XDisplayFromHandle(void *Handle)
{
	return GDK_WINDOW_XDISPLAY(gtk_widget_get_window(GTK_WIDGET(Handle)));
}

Window XWindowFromHandle(void *Handle)
{
	return GDK_WINDOW_XID(gtk_widget_get_window(GTK_WIDGET(Handle)));
}

//static GdkGrabStatus gdk_x11_device_core_grab   (GdkDevice     *device,
//                                                 GdkWindow     *window,
//                                                 gboolean       owner_events,
//                                                 GdkEventMask   event_mask,
//                                                 GdkWindow     *confine_to,
//                                                 GdkCursor     *cursor,
//                                                 guint32        time_);
//static GdkGrabStatus gdk_x11_device_xi2_grab   (GdkDevice     *device,
//                                                GdkWindow     *window,
//                                                gboolean       owner_events,
//                                                GdkEventMask   event_mask,
//                                                GdkWindow     *confine_to,
//                                                GdkCursor     *cursor,
//                                                guint32        time_);
#endif

//bool IsPointerGrabbed(Display *dpy, Window win)
bool IsPointerGrabbed()
{
	/*
	int status = XChangeActivePointerGrab(dpy
		, 0
		, None
		, CurrentTime
		);

	fprintf(stderr
		, "IsPointerGrabbed: %d %d\n", status, status == BadValue
		);

	if (status == BadValue)
		return false;

	return true;
	*/

	Display *dpy = XOpenDisplay("");

	/*
	int deviceid;
	XIGetClientPointer(dpy, win, &deviceid);
	XIEventMask mask = { deviceid, 0, (unsigned char*)"" };
	int status = XIGrabDevice (
		//GDK_DISPLAY_XDISPLAY (display)
		dpy

		, deviceid

		//, win
		//, xwindow
		, DefaultRootWindow(dpy)

		, CurrentTime

		, None

		, GrabModeAsync
		, GrabModeAsync

		, True

		, &mask
		);

	fprintf(stderr
		, "IsPointerGrabbed 0: status %d, GrabSuccess %d\n", status, status == GrabSuccess
		);
	*/

	int status = XGrabPointer(dpy

		//, win
		, RootWindow(dpy, DefaultScreen(dpy))
		//, DefaultRootWindow(dpy)

		, True

		, 0
		//, ButtonReleaseMask | ButtonMotionMask | ButtonPressMask

		, GrabModeAsync
		, GrabModeAsync

		, None
		//, win

		, None

		, CurrentTime
		//, 1
		);

	//XCloseDisplay(dpy);

	fprintf(stderr
		, "IsPointerGrabbed: status %d, GrabSuccess %d\n", status, status == GrabSuccess
		);

	//if (status == AlreadyGrabbed)
		//return true;

	if (status == GrabSuccess)
	{
		XUngrabPointer(dpy, CurrentTime);
		XFlush(dpy);
	}

	XCloseDisplay(dpy);

	return status == AlreadyGrabbed;
}

void MyFrame::Grab(bool grab)
{
#ifdef __WXGTK__

	//wxMessageBox(wxString::Format("test"), "");
	Display *dpy = XDisplayFromHandle(GetHandle());
	//Window rwin = RootWindow(dpy, DefaultScreen (dpy));
	Window win = XWindowFromHandle(GetHandle());
	//Window win2 = XWindowFromHandle(panel->GetHandle());
	//GdkWindow *win2 = XWindowFromHandle(GetHandle());
	//bool free = !X11Utils::IsPointerGrabbed(dpy, win);


	//GtkWidget* widget = output->GetHandle();
	GtkWidget *widget = GetHandle();
	//GtkWidget *widget = panel->GetHandle();
	gtk_widget_realize(widget);   // Mandatory. Otherwise it segfaults.

	//GtkWindow *gtk_window = gtk_widget_get_window(widget);
	GdkWindow *window = gtk_widget_get_window(widget);
	//GdkWindow *window = GDK_WINDOW_XWINDOW(((GtkWidget*)(GetHandle()))->window);
	//GdkWindow* window = GTK_PIZZA(output->m_wxwindow)->bin_window;
	//Window window = GDK_WINDOW_XWINDOW(widget->window);   // Window is a typedef for XID, which is a typedef for unsigned int

	Window xwindow = GDK_WINDOW_XID (window);

	GdkDisplay *display = gdk_window_get_display(window);
	//Display *display = GDK_WINDOW_XDISPLAY( widget->window );

	GdkDeviceManager *device_manager = gdk_display_get_device_manager(display);	
	

	/*
	GdkDevice *device;
	GList *devices, *dev;
	devices = gdk_device_manager_list_devices (device_manager, GDK_DEVICE_TYPE_MASTER);
	//devices = gdk_device_manager_list_devices (device_manager, GDK_DEVICE_TYPE_SLAVE);

	for (dev = devices; dev; dev = dev->next)
	{
		device = (GdkDevice*)dev->data;

		//if (gdk_device_get_source (device) != GDK_SOURCE_MOUSE)
			//continue;

		fprintf(stderr,
			"name: %s\n"

			"GDK_IS_X11_DEVICE_XI2: %d\n"

			//, device->name
			, gdk_device_get_name(device)

			, GDK_IS_X11_DEVICE_XI2(device)
			);
      //display->core_pointer = device;
      //break;
	}
	g_list_free (devices);
	*/


	GdkDevice *device = gdk_device_manager_get_client_pointer(device_manager);

/*
	fprintf(stderr,

		"GDK_BACKEND: %d\n"

		"GDK_IS_X11_DISPLAY: %d\n"
		//"GDK_IS_QUARTZ_DISPLAY: %d\n"
		//"GDK_IS_BROADWAY_DISPLAY: %d\n"
		//"GDK_IS_WIN32_DISPLAY: %d\n"

		"GDK_TYPE_X11_DEVICE_CORE: %d\n"
		"GDK_IS_X11_DEVICE_XI2: %d\n"

		"gdk_window_ensure_native: %d\n"

		, g_getenv ("GDK_BACKEND")

		, GDK_IS_X11_DISPLAY(display)
		//, GDK_IS_QUARTZ_DISPLAY(display)
		//, GDK_IS_BROADWAY_DISPLAY(display)
		//, GDK_IS_WIN32_DISPLAY(display)

		, GDK_IS_X11_DEVICE_CORE(device)
		, GDK_IS_X11_DEVICE_XI2(device)

		, gdk_window_ensure_native(window)
		);
*/

	/*
	wxWindowList::compatibility_iterator node = wxTopLevelWindows.GetFirst();
	while (node)
	{
		wxWindow* win = node->GetData();
		// do something with "win"
		fprintf(stderr,
			"wxWindowList: %d %d %d | %u %u %u | %u %u\n"
			, GetHandle()
			, panel->GetHandle()
			, win->GetHandle()
			
			, RootWindow(dpy, DefaultScreen (dpy))
			, XWindowFromHandle(GetHandle())
			, XWindowFromHandle(panel->GetHandle())

			, gtk_widget_get_window((GtkWidget*)GetHandle())
			, gtk_widget_get_window((GtkWidget*)panel->GetHandle())
			);

		node = node->GetNext();
	}
	*/


	bool free = !IsPointerGrabbed();
	//bool free = !gdk_pointer_is_grabbed();
	//bool free = !gdk_display_pointer_is_grabbed(display);
	fprintf(stderr
		, "free: %d\n"
		, free
		);

	//if (grab)
	if (free)
	//if (true)
	{
		/*


	const GdkEventMask mask = GdkEventMask(
		//GDK_BUTTON_PRESS_MASK
		//| GDK_BUTTON_RELEASE_MASK
		//GDK_POINTER_MOTION_HINT_MASK
		//| GDK_POINTER_MOTION_MASK
		//GDK_ALL_EVENTS_MASK
		0
		);

	//gdk_device_grab(device
	//	, window

	//	, GDK_OWNERSHIP_NONE

	//	, true
	//	//, false

	//	, mask

	//	, NULL

	//	, unsigned(GDK_CURRENT_TIME));

	unsigned int xmask = 
		ButtonPressMask
		| ButtonReleaseMask
		| PointerMotionMask
		| PointerMotionHintMask
		| FocusChangeMask
		| EnterWindowMask
		| LeaveWindowMask

		//NoEventMask

		//0
		;  


		//gtk_grab_add( widget );

		int status = gdk_pointer_grab(window
			, true
			//, false

			//, mask
			, GdkEventMask(0)

			, window
			//, NULL

			, NULL
			, (guint32)GDK_CURRENT_TIME);
		*/

		/*

		status = gdk_x11_device_xi2_grab(device
		//status = gdk_x11_device_core_grab(device
			, window

			, true
			//, false

			//, mask
			, GdkEventMask(0)

			, window
			//, NULL

			, NULL
			, (guint32)GDK_CURRENT_TIME);
					*/


		/*
		//GDK_GRAB_SUCCESS

		fprintf(stderr
			, "gdk_pointer_grab: %d\n"
			"gdk_pointer_is_grabbed: %d\n"
			, status
			, gdk_pointer_is_grabbed()
			);
		*/


	
/*
		*/
		int status = XGrabPointer(dpy
			//, RootWindow (dpy, DefaultScreen (dpy))
			, DefaultRootWindow(dpy)
			//, rwin
			//, win
			//, win2
			//, xwindow

			, True
			//, False

			//, xmask
			, 0

			, GrabModeAsync
			//, GrabModeSync

			, GrabModeAsync

			, win
			//, win2
			//, xwindow
			//, None

			, None
			, CurrentTime);

		fprintf(stderr
			, "XGrabPointer: status %d, GrabSuccess %d\n", status, status == GrabSuccess
			//, IsPointerGrabbed(dpy, win)
			);
	


		/*

			//GdkX11DeviceXI2 *device_xi2 = GDK_X11_DEVICE_XI2 (device);

			int deviceid;
			XIGetClientPointer(dpy, win, &deviceid);

			//const GdkEventMask event_mask = GdkEventMask(0);

			XIEventMask mask = { deviceid, 0, (unsigned char*)"" };
			//XIEventMask mask;
			//mask.deviceid = deviceid;
			//mask.mask = gdk_device_xi2_translate_event_mask (event_mask, &mask.mask_len);
			//mask.mask_len = XIMaskLen(XI_LASTEVENT);
			//mask.mask = calloc(mask.mask_len, sizeof(char));

			//EXIEventMask mask;
			//EXIMaskSetup(&mask, deviceid,
			//	ButtonPressMask | ButtonReleaseMask | PointerMotionMask);

			//XIAllMasterDevices
			//XIMasterPointer
	
			int status = XIGrabDevice (
				//GDK_DISPLAY_XDISPLAY (display)
				dpy

				//, device_xi2->device_id
				, deviceid
				//, XIMasterPointer
				//, 2
				//, 8

				, win
				//, xwindow

				, CurrentTime

				, None

				, GrabModeAsync
				, GrabModeAsync

				, True

				//, 0
				, &mask
				);

		fprintf(stderr,
			"XIGrabDevice: deviceid %d, status %d, GrabSuccess %d\n"
			//"IsPointerGrabbed: %d\n"
			
			, deviceid, status, status == GrabSuccess
			//, IsPointerGrabbed(dpy, win)
			);
				*/
		
		//IsPointerGrabbed(dpy, win);
		//XRaiseWindow(dpy, win);
		//XSync(dpy, true);


		//CaptureMouse();
	}
	else
	{
		/*

		gdk_pointer_ungrab((guint32)GDK_CURRENT_TIME);
		//gtk_grab_remove( m_widget );

		fprintf(stderr
			, "gdk_pointer_is_grabbed: %d\n"
			, gdk_pointer_is_grabbed()
			);
		*/

		XUngrabPointer(dpy, CurrentTime);
		//XIUngrabDevice(dpy, 2, CurrentTime);

		//ReleaseMouse();
	}

	//IsPointerGrabbed();

	//gdk_window_move(window, 200, 200);
	//gdk_window_move_resize(window, 0, 0, 200, 200);
	//gtk_window_set_position(widget, GTK_WIN_POS_MOUSE);
	//gtk_widget_set_uposition(window, 0, 0);

	//XResizeWindow(dpy, rwin, 200, 200);
	//XResizeWindow(dpy, win, 200, 200);
	//XResizeWindow(dpy, win2, 200, 200);

	//gint w, h;
	//gtk_window_get_size(window, &w, &h);
	//	fprintf(stderr
	//		, "w: %d\n"
	//		"h: %d\n"
	//		, w
	//		, h
	//		);

/*
	GdkRectangle r;
	gdk_window_get_frame_extents(window, &r);
		fprintf(stderr
			, "x: %d\n"
			"y: %d\n"
			"w: %d\n"
			"h: %d\n"
			, r.x
			, r.y
			, r.width
			, r.height
			);
*/

	/*
	Window rw;
	int x, y;
	unsigned int w, h, b, d;
	XGetGeometry(dpy, win2, &rw, &x, &y, &w, &h, &b, &d);

		fprintf(stderr
			, "x: %d\n"
			"y: %d\n"
			"w: %d\n"
			"h: %d\n"
			, x
			, y
			, w
			, h
			);
	*/

#endif
}


// event handlers

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // true is to force the frame to close
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(wxString::Format
                 (
                    "Welcome to %s!\n"
                    "\n"
                    "This is the minimal wxWidgets sample\n"
                    "running under %s.",
                    wxVERSION_STRING,
                    wxGetOsDescription()
                 ),
                 "About wxWidgets minimal sample",
                 wxOK | wxICON_INFORMATION,
                 this);
}
