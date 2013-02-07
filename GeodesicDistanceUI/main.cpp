#include <gtkmm.h>
#include "geodesic_distance_ui.h"
 
int main(int argc, char *argv[])
{
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "geodesic.distance");

  Geodesic_Distance_UI window;

  return app->run(window);
}
