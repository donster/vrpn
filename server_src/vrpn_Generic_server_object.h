#ifndef VRPN_GENERIC_SERVER_OBJECT_H
#define VRPN_GENERIC_SERVER_OBJECT_H

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

#include "vrpn_MainloopContainer.h"

#include "vrpn_Configure.h"
#ifdef	sgi
#define	SGI_BDBOX
#endif
#include "vrpn_Connection.h"
#include "vrpn_Button.h"
#include "vrpn_Tracker.h"
#include "vrpn_Analog.h"
#include "vrpn_Analog_Output.h"
#include "vrpn_Poser.h"
#include "vrpn_3Space.h"
#include "vrpn_Tracker_Fastrak.h"
#include "vrpn_Tracker_Isotrak.h"
#include "vrpn_Tracker_Liberty.h"
#include "vrpn_Tracker_LibertyHS.h"
#include "vrpn_Tracker_3DMouse.h"
#include "vrpn_Flock.h"
#include "vrpn_Flock_Parallel.h"
#include "vrpn_Dyna.h"
#include "vrpn_Sound.h"
#include "vrpn_raw_sgibox.h" // for access to the SGI button & dial box connected to the serial port of an linux PC
#ifdef	SGI_BDBOX
#include "vrpn_sgibox.h" //for access to the B&D box connected to an SGI via the IRIX GL drivers
#endif
#include "vrpn_UNC_Joystick.h"
#include "vrpn_Joylin.h"
#include "vrpn_JoyFly.h"
#include "vrpn_CerealBox.h"
#include "vrpn_Tracker_AnalogFly.h"
#include "vrpn_Tracker_ButtonFly.h"
#include "vrpn_Magellan.h"
#include "vrpn_Spaceball.h"
#include "vrpn_ImmersionBox.h"
#include "vrpn_Analog_Radamec_SPI.h"
#include "vrpn_Zaber.h"
#include "vrpn_BiosciencesTools.h"
#include "vrpn_IDEA.h"
#include "vrpn_Wanda.h"
#include "vrpn_Analog_5dt.h"
#include "vrpn_Tng3.h"
#include "vrpn_Tracker_isense.h"
#include "vrpn_DirectXFFJoystick.h"
#include "vrpn_DirectXRumblePad.h"
#include "vrpn_XInputGamepad.h"
#include "vrpn_Joywin32.h"
#include "vrpn_GlobalHapticsOrb.h"
#ifdef VRPN_USE_PHANTOM_SERVER
#include "vrpn_Phantom.h"
#endif
#include "vrpn_ADBox.h"
#include "vrpn_VPJoystick.h"
#include "vrpn_Tracker_DTrack.h"
#include "vrpn_NationalInstruments.h"
#include "vrpn_Poser_Analog.h"
#include "vrpn_nikon_controls.h"
#include "vrpn_Poser_Tek4662.h"
#include "vrpn_Mouse.h"
#include "vrpn_DevInput.h"
#include "vrpn_Tracker_Crossbow.h"
#include "vrpn_3DMicroscribe.h"
#include "vrpn_5DT16.h"
#include "vrpn_Auxiliary_Logger.h"
#include "vrpn_Imager_Stream_Buffer.h"
// added by Tom dw
#include "vrpn_Button_USB.h"
// added by Bill West
#include "vrpn_Analog_USDigital_A2.h"
#include "vrpn_Button_NI_DIO24.h"
#include "vrpn_Tracker_PhaseSpace.h"
#include "vrpn_Xkeys.h"
#include "vrpn_3DConnexion.h"
#include "vrpn_Keyboard.h"
#include "vrpn_Tracker_MotionNode.h"
#include "vrpn_Tracker_GPS.h"
#include "vrpn_Tracker_NDI_Polaris.h"
#include "vrpn_WiiMote.h"
#include "vrpn_Tracker_WiimoteHead.h"
#include "vrpn_Freespace.h"
#include "vrpn_DreamCheeky.h"
#include "vrpn_Tracker_NovintFalcon.h"
#include "vrpn_Tracker_TrivisioColibri.h" // added by David Borland
#include "vrpn_LUDL.h"
#include "vrpn_Tracker_GameTrak.h"
#include "vrpn_Tracker_SpacePoint.h"
#include "vrpn_Analog_5dtUSB.h"
#include "vrpn_Tracker_RazerHydra.h"
#include "vrpn_Tracker_zSight.h" // added by David Borland
#include "vrpn_Tracker_ViewPoint.h" // added by David Borland

// BUW additions
#include "vrpn_Atmel.h"
#include "vrpn_inertiamouse.h"
#include "vrpn_Event_Mouse.h"
//#include "vrpn_Cirque_Extended.h"

// Polhemus additions
#include "vrpn_Tracker_PDI.h"

#ifdef VRPN_USE_JSONNET
#include "vrpn_Tracker_JsonNet.h"
#endif

#ifdef VRPN_INCLUDE_TIMECODE_SERVER
#include "timecode_generator_server\vrpn_timecode_generator.h"
#endif

const int VRPN_GSO_MAX_NDI_POLARIS_RIGIDBODIES = 20; //FIXME find out from the NDI specs if there is a maximum;

class vrpn_Generic_Server_Object
{
  public:
    vrpn_Generic_Server_Object (vrpn_Connection *connection_to_use, const char *config_file_name = "vrpn.cfg", int port = vrpn_DEFAULT_LISTEN_PORT_NO, bool be_verbose = false, bool bail_on_open_error = false);
    ~vrpn_Generic_Server_Object();

    void mainloop (void);
    inline bool doing_okay (void) const {
      return d_doing_okay;
    }

  protected:
    vrpn_Connection *connection;          //< Connection to communicate on
    bool            d_doing_okay;         //< Is the object working okay?
    bool            verbose;              //< Should we print lots of info?
    bool            d_bail_on_open_error; //< Should we bail if we have an error opening a device?

    // Lists of devices
    vrpn_MainloopContainer _devices;

    void closeDevices (void);

    // Helper functions for the functions below
    int get_AFline (char *line, vrpn_TAF_axis *axis);
    int	get_poser_axis_line (FILE *config_file, const char *axis_name, vrpn_PA_axis *axis, vrpn_float64 *min, vrpn_float64 *max);

    // Functions to parse each kind of device from the configuration file
    // and create a device of that type linked to the appropriate lists.
    int setup_raw_SGIBox (char * & pch, char * line, FILE * /*config_file*/);
    int setup_SGIBox (char * & pch, char * line, FILE * /*config_file*/);
    int setup_Tracker_AnalogFly (char * & pch, char * line, FILE * config_file);
    int setup_Tracker_ButtonFly (char * & pch, char * line, FILE * config_file);
    int setup_Joystick (char * & pch, char * line, FILE * /*config_file*/);
    int setup_Example_Button (char * & pch, char * line, FILE * /*config_file*/);
    int setup_Example_Dial (char * & pch, char * line, FILE * /*config_file*/);
    int setup_CerealBox (char * & pch, char * line, FILE * /*config_file*/);
    int setup_Magellan (char * & pch, char * line, FILE * /*config_file*/);
    int setup_Spaceball (char * & pch, char * line, FILE * /*config_file*/);
    int setup_Radamec_SPI (char * & pch, char * line, FILE * /*config_file*/);
    int setup_Zaber (char * & pch, char * line, FILE * /*config_file*/);
    int setup_BiosciencesTools (char * & pch, char * line, FILE * /*config_file*/);
    int setup_IDEA (char * & pch, char * line, FILE * /*config_file*/);
    int setup_NationalInstruments (char * & pch, char * line, FILE * /*config_file*/);
    int setup_NationalInstrumentsOutput (char * & pch, char * line, FILE * /*config_file*/);
    int setup_ImmersionBox (char * & pch, char * line, FILE * /*config_file*/);
    int setup_5dt (char * & pch, char * line, FILE * /*config_file*/);
    int setup_Wanda (char * & pch, char * line, FILE * /*config_file*/);
    int setup_Tracker_Dyna (char * & pch, char * line, FILE * /*config_file*/);
    int setup_Tracker_Fastrak (char * & pch, char * line, FILE * config_file);
    int setup_Tracker_Isotrak (char * & pch, char * line, FILE * config_file);
    int setup_Tracker_Liberty (char * & pch, char * line, FILE * config_file);
    int setup_Tracker_LibertyHS (char * & pch, char * line, FILE * config_file);
    int setup_Tracker_3Space (char * & pch, char * line, FILE * /*config_file*/);
    int setup_Tracker_Flock (char * & pch, char * line, FILE * /*config_file*/);
    int setup_Tracker_Flock_Parallel (char * & pch, char * line, FILE * /*config_file*/);
    int setup_Tracker_NULL (char * & pch, char * line, FILE * /*config_file*/);
    int setup_Button_Python (char * & pch, char * line, FILE * /*config_file*/);
    int setup_Button_SerialMouse (char * & pch, char * line, FILE * /*config_file*/);
    int setup_Button_PinchGlove (char* &pch, char *line, FILE *config_file);
    int setup_Joylin (char * & pch, char * line, FILE * /*config_file*/);
    int setup_Joywin32 (char * & pch, char * line, FILE * /*config_file*/);
    int setup_Tng3 (char * & pch, char * line, FILE * /*config_file*/);
    int setup_Tracker_InterSense (char * &pch, char *line, FILE * config_file);
    int setup_DirectXFFJoystick (char * & pch, char * line, FILE * /*config_file*/);
    int setup_RumblePad (char * & pch, char * line, FILE * /*config_file*/);
    int setup_XInputPad (char * & pch, char * line, FILE * /*config_file*/);
    int setup_GlobalHapticsOrb (char * & pch, char * line, FILE * /*config_file*/);
    int setup_ADBox (char* &pch, char *line, FILE *config_file);
    int setup_VPJoystick (char* &pch, char *line, FILE *config_file);
    int setup_DTrack (char* &pch, char* line, FILE* config_file);
    int setup_Poser_Analog (char * & pch, char * line, FILE * config_file);
    int setup_nikon_controls (char * & pch, char * line, FILE * /*config_file*/);
    int setup_Poser_Tek4662 (char * & pch, char * line, FILE * /*config_file*/);
    int setup_Timecode_Generator (char * & pch, char * line, FILE * /*config_file*/);
    int setup_Phantom (char * & pch, char * line, FILE * /*config_file*/);
    int setup_JoyFly (char * & pch, char * line, FILE * /*config_file*/);
    int setup_Tracker_3DMouse (char * & pch, char * line, FILE * /*config_file*/);
    int setup_Mouse (char * & pch, char * line, FILE * /*config_file*/);
    int setup_DevInput (char * & pch, char * line, FILE * /*config_file*/);
    int setup_Tracker_Crossbow (char * & pch, char * line, FILE * /*config_file*/);
    int setup_3DMicroscribe (char * & pch, char * line, FILE * /*config_file*/);
    int setup_5dt16 (char * & pch, char * line, FILE * /*config_file*/);
    int setup_Button_5DT_Server (char * & pch, char * line, FILE * /*config_file*/);
    int setup_Keyboard (char * & pch, char * line, FILE * /*config_file*/);
    int setup_Button_USB (char * & pch, char * line, FILE * /*config_file*/);
    int setup_Analog_USDigital_A2 (char * & pch, char * line, FILE * /*config_file*/) ;
    int setup_Button_NI_DIO24 (char * & pch, char * line, FILE * /*config_file*/) ;
    int setup_Tracker_PhaseSpace (char * & pch, char * line, FILE * /*config_file*/) ;
    int setup_Tracker_NDI_Polaris (char * & pch, char * line, FILE * config_file) ;
    int setup_Logger (char * & pch, char * line, FILE * /*config_file*/) ;
    int setup_ImageStream (char * & pch, char * line, FILE * /*config_file*/) ;
    int setup_Xkeys_Desktop (char * & pch, char * line, FILE * /*config_file*/) ;
    int setup_Xkeys_Pro (char * & pch, char * line, FILE * /*config_file*/) ;
    int setup_Xkeys_Joystick (char * & pch, char * line, FILE * /*config_file*/) ;
    int setup_Xkeys_Jog_And_Shuttle (char * & pch, char * line, FILE * /*config_file*/) ;
    int setup_3DConnexion_Navigator (char * & pch, char * line, FILE * /*config_file*/) ;
    int setup_3DConnexion_Navigator_for_Notebooks (char * & pch, char * line, FILE * /*config_file*/) ;
    int setup_3DConnexion_Traveler (char * & pch, char * line, FILE * /*config_file*/) ;
    int setup_3DConnexion_SpaceMouse (char * & pch, char * line, FILE * /*config_file*/) ;
    int setup_3DConnexion_SpaceExplorer (char * & pch, char * line, FILE * /*config_file*/) ;
    int setup_3DConnexion_SpaceBall5000 (char * & pch, char * line, FILE * /*config_file*/) ;
    int setup_Tracker_MotionNode (char * & pch, char * line, FILE * /*config_file*/);
    int setup_Tracker_GPS (char * & pch, char * line, FILE * /*config_file*/);
    int setup_WiiMote (char * & pch, char * line, FILE * /*config_file*/);
    int setup_SpacePoint (char * & pch, char * line, FILE * /*config_file*/);
    int setup_Tracker_WiimoteHead (char * & pch, char * line, FILE * /*config_file*/);
    int setup_Freespace (char * & pch, char * line, FILE * /*config_file*/);
    int setup_DreamCheeky (char * & pch, char * line, FILE * /*config_file*/) ;
    int setup_Tracker_NovintFalcon (char * & pch, char * line, FILE * /*config_file*/);
    int setup_Tracker_TrivisioColibri (char * &pch, char * line, FILE * /*config_file*/);
    int setup_Tracker_GameTrak (char *pch, char *line, FILE * config_file);
    int setup_LUDL_USBMAC6000 (char * &pch, char * line, FILE * /*config_file*/);
    int setup_Analog_5dtUSB_Glove5Left (char * &pch, char * line, FILE * /*config_file*/);
    int setup_Analog_5dtUSB_Glove5Right (char * &pch, char * line, FILE * /*config_file*/);
    int setup_Analog_5dtUSB_Glove14Left (char * &pch, char * line, FILE * /*config_file*/);
    int setup_Analog_5dtUSB_Glove14Right (char * &pch, char * line, FILE * /*config_file*/);
    int setup_Tracker_RazerHydra (char * &pch, char * line, FILE * /*config_file*/);
    int setup_Tracker_zSight (char * &pch, char * line, FILE * /*config_file*/);
    int setup_Tracker_ViewPoint (char * &pch, char * line, FILE * /*config_file*/);

    // BUW additions
    int setup_Atmel (char* &pch, char *line, FILE *config_file);
    int setup_Event_Mouse (char* &pch, char *line, FILE *config_file);
    int setup_inertiamouse (char * & pch, char * line, FILE * /*config_file*/);

    // Polhemus additions
    int setup_Tracker_G4(char* &pch, char* line, FILE* config_file); 
    int setup_Tracker_LibertyPDI(char* &pch, char* line, FILE* config_file); 
    int setup_Tracker_FastrakPDI(char* &pch, char* line, FILE* config_file);

#ifdef VRPN_USE_JSONNET
    int setup_Tracker_JsonNet (char* &pch, char* line, FILE* config_file);
#endif

};

#endif

