#include <gtkmm.h>
#include "geodesic_distance_ui.h"
#include "rig/patch_weight_rig.h"
#include "rig/patchmatch_rig.h"
 
int main(int argc, char *argv[])
{
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "geodesic.distance");

	Geodesic_Distance_UI window;

	// Initialize rigs.
	window.add_rig(new Patch_Weight_Rig, "Geodesic Weight");
	window.add_rig(new PatchMatch_Rig, "PatchMatch");
	window.initialize_rigs();

	return app->run(window);
}
