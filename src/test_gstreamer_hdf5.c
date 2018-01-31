#include "hdf5.h"
#include <stdio.h>
#include <stdlib.h>
#include <gst/gst.h>

#define FILE            "dset.h5"
#define DATASET         "dset"
#define DIM0            10
#define DIM1            2

int main(int argc, char *argv[]) {
  /*
   * LOAD HDF5 FILE
   */
  hid_t file, dset;
  herr_t status;
  int rdata[DIM0][DIM1], i, j;

  // Open file and dataset using the default properties.
  file = H5Fopen (FILE, H5F_ACC_RDONLY, H5P_DEFAULT);
  dset = H5Dopen (file, "dset", H5P_DEFAULT);

  // Read the data using the default properties.
  status = H5Dread(dset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, rdata[0]);

  /* // Output the data to the screen. */
  /* printf ("%s:\n", DATASET); */
  /* for (i=0; i<DIM0; i++) { */
  /*   printf (" ["); */
  /*   for (j=0; j<DIM1; j++) */
  /*     printf (" %3d", rdata[i][j]); */
  /*   printf ("]\n"); */
  /* } */

  // Close and release resources.
  status = H5Dclose (dset);
  status = H5Fclose (file);




  /*
   * Gstreamer Pipe
   */
  GstElement *pipeline, *source, *sink;
  GstBus *bus;
  GstMessage *msg;
  GstStateChangeReturn ret;

  /* Initialize GStreamer */
  gst_init (&argc, &argv);

  /* Create the elements */
  source = gst_element_factory_make ("videotestsrc", "source");
  sink = gst_element_factory_make ("autovideosink", "sink");

  /* Create the empty pipeline */
  pipeline = gst_pipeline_new ("test-pipeline");

  if (!pipeline || !source || !sink) {
    g_printerr ("Not all elements could be created.\n");
    return -1;
  }

  /* Build the pipeline */
  gst_bin_add_many (GST_BIN (pipeline), source, sink, NULL);
  if (gst_element_link (source, sink) != TRUE) {
    g_printerr ("Elements could not be linked.\n");
    gst_object_unref (pipeline);
    return -1;
  }

  for (i=0; i<DIM0; i++)
    {
      for (j=0; j<DIM1; j++)
	{
	  /* paused */
	  ret = gst_element_set_state (pipeline, GST_STATE_PAUSED);
	  if (ret == GST_STATE_CHANGE_FAILURE) {
	    g_printerr ("Unable to set the pipeline to the paused state.\n");
	    gst_object_unref (pipeline);
	    return -1;
	  }

	  /* Modify the source's properties */
	  g_object_set (source, "pattern", rdata[i][j]%25, NULL);

	  /* Start playing */
	  ret = gst_element_set_state (pipeline, GST_STATE_PLAYING);
	  if (ret == GST_STATE_CHANGE_FAILURE) {
	    g_printerr ("Unable to set the pipeline to the playing state.\n");
	    gst_object_unref (pipeline);
	    return -1;
	  }
	  sleep(1);
	}
    }

  /* Stop playing */
  ret = gst_element_set_state (pipeline, GST_STATE_NULL);
  if (ret == GST_STATE_CHANGE_FAILURE) {
    g_printerr ("Unable to set the pipeline to the null state.\n");
    gst_object_unref (pipeline);
    return -1;
  }

  /* Wait until error or EOS */
  bus = gst_element_get_bus (pipeline);
  msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

  /* Parse message */
  if (msg != NULL) {
    GError *err;
    gchar *debug_info;

    switch (GST_MESSAGE_TYPE (msg)) {
    case GST_MESSAGE_ERROR:
      gst_message_parse_error (msg, &err, &debug_info);
      g_printerr ("Error received from element %s: %s\n", GST_OBJECT_NAME (msg->src), err->message);
      g_printerr ("Debugging information: %s\n", debug_info ? debug_info : "none");
      g_clear_error (&err);
      g_free (debug_info);
      break;
    case GST_MESSAGE_EOS:
      g_print ("End-Of-Stream reached.\n");
      break;
    default:
      /* We should not reach here because we only asked for ERRORs and EOS */
      g_printerr ("Unexpected message received.\n");
      break;
    }
    gst_message_unref (msg);
  }

  /* Free resources */
  gst_object_unref (bus);
  gst_element_set_state (pipeline, GST_STATE_NULL);
  gst_object_unref (pipeline);

  return 0;
}
