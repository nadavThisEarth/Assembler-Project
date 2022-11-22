#include "preprocessor.h"

#include "passes_manager.h"
int main(int argc, char *argv[])
{

  preprocessor(argc, argv); /*copy the content of the file and spread macros into .am file*/
  manage_passes(argc, argv);/* pass files to first and second pass for analysis    */
  return 0;
}
