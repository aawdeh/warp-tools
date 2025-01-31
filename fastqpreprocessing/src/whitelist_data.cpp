#include "whitelist_data.h"

#include <fstream>
#include <iostream>

WhiteListData readWhiteList(std::string const& white_list_file)
{
  const char ATCG[] = {'A', 'C', 'G', 'T', 'N'};

  std::ifstream file(white_list_file);
  if (!file.is_open())
    crash("Couldn't open whitelist file " + white_list_file);

  WhiteListData white_list_data;
  int k = 0;
  // read data from file object and put it into string.
  for (std::string tp; getline(file, tp); )
  {
    white_list_data.barcodes.push_back(tp);

    for (unsigned int i=0; i < tp.size(); i++)
    {
      for (int j=0; j < 5; j++)
      {
        char c = tp[i];
        tp[i] = ATCG[j];
        // If the mutation we're writing is already present, we just overwrite
        // what was there with the current.
        // This is done to have the same values for corrected barcodes
        // as in the python implementation.
        white_list_data.mutations[tp] = k;
        tp[i] = c;
      }
    }

    // -1 suggests it is already a whitelisted barcode
    // This is used, instead of the actual index, because when
    // the barcode is seen with -1 then no correction is necessary.
    // Avoids lots of map lookups, as most barcodes are not erroneous.
    white_list_data.mutations[tp] = -1;
    k++;
  }

  return white_list_data;
}
