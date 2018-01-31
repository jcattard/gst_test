#include "hdf5.h"
#define FILE "dset.h5"

int main() {
  hid_t       file_id, dataset_id, dataspace_id;  /* identifiers */
  hsize_t     dims[2];
  herr_t      status;
  int         i, j, dset_data[10][2];

   /* Initialize the dataset. */
   for (i = 0; i < 10; i++)
      for (j = 0; j < 2; j++)
         dset_data[i][j] = i * 6 + j + 1;

  /* Create a new file using default properties. */
  file_id = H5Fcreate(FILE, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

  /* Create the data space for the dataset. */
  dims[0] = 10; 
  dims[1] = 2; 
  dataspace_id = H5Screate_simple(2, dims, NULL);

  /* Create the dataset. */
  dataset_id = H5Dcreate2(file_id, "/dset", H5T_STD_I32BE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

   /* Write the dataset. */
   status = H5Dwrite(dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, dset_data);
  
  /* End access to the dataset and release resources used by it. */
  status = H5Dclose(dataset_id);
  
  /* Terminate access to the data space. */ 
  status = H5Sclose(dataspace_id);

  /* Close the file. */
  status = H5Fclose(file_id);
}
