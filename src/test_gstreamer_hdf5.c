#include "hdf5.h"
#include <stdio.h>
#include <stdlib.h>

#define FILE            "dset.h5"
#define DATASET         "dset"
#define DIM0            10
#define DIM1            2

int main (void)
{
  hid_t file, dset;
  herr_t status;
  int rdata[DIM0][DIM1], i, j;

  // Open file and dataset using the default properties.
  file = H5Fopen (FILE, H5F_ACC_RDONLY, H5P_DEFAULT);
  dset = H5Dopen (file, "dset", H5P_DEFAULT);

  // Read the data using the default properties.
  status = H5Dread(dset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, rdata[0]);

  // Output the data to the screen.
  printf ("%s:\n", DATASET);
  for (i=0; i<DIM0; i++) {
    printf (" [");
    for (j=0; j<DIM1; j++)
      printf (" %3d", rdata[i][j]);
    printf ("]\n");
  }

  // Close and release resources.
  status = H5Dclose (dset);
  status = H5Fclose (file);

  return 0;
}
