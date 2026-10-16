#include "notes.h"
#include <gtk/gtk.h>

static gboolean on_key_press(GtkEventControllerKey *controller, guint keyval,
                             guint keycode, GdkModifierType state,
                             gpointer user_data) {
  (void)controller;
  (void)keycode;
  (void)state;
  (void)user_data;

  keyval = gdk_keyval_to_lower(keyval);

  for (size_t i = 0; i < sizeof(notes) / sizeof(notes[0]); i++) {
    if ((guint)(unsigned char)notes[i].key == keyval) {
      g_print("Key: %c | Frequency: %.2f Hz\n", notes[i].key,
              notes[i].frequency);
      return TRUE;
    }
  }

  return FALSE;
}

static void activate(GtkApplication *app, gpointer user_data) {
  (void)user_data;

  GtkWidget *window = gtk_application_window_new(app);

  gtk_window_set_title(GTK_WINDOW(window), "csynth");
  gtk_window_set_default_size(GTK_WINDOW(window), 640, 400);

  GtkEventController *keyboard = gtk_event_controller_key_new();

  g_signal_connect(keyboard, "key-pressed", G_CALLBACK(on_key_press), NULL);

  gtk_widget_add_controller(window, keyboard);

  gtk_window_present(GTK_WINDOW(window));
}

int main(void) {
  GtkApplication *app =
      gtk_application_new("com.example.csynth", G_APPLICATION_DEFAULT_FLAGS);

  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

  int status = g_application_run(G_APPLICATION(app), 0, NULL);

  g_object_unref(app);

  return status;
}
