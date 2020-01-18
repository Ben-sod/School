int set_temp_from_ports(temp_t *temp){
  if(THERMO_SENSOR_PORT>64000){
    return 1;
  }
  else{
    short rounding=THERMO_SENSOR_PORT%64;
    //if the remainder is less than 32, the round down.
    if(rounding>32){
      short temperature=THERMO_SENSOR_PORT/64;
    }
    //if the remainder is greater than 32, then round up (add one).
    else{
      short temperature=THERMO_SENSOR_PORT/64+1;
    }
    //account for offset of -50.0 C
    temperature=temperature-500;
    //convert to fahrenheit if necessary
    if(&temp->is_fahrenheit==1){
      temperature= (temperature * 90) / 50 + 320;
    }
  }
  //setting the temperature field on the temp_t struct
  &temp->tenths_degrees=temperature;
  return 0
}


int set_display_from_temp(temp_t temp, int *display){
  //allocating and declaring array of masks
  int *masks = malloc(10*sizeof(int));
  int masks = {0b0111111, //0
               0b0000110, //1
               0b1011011, //2
               0b1001111, //3
               0b1100110, //4
               0b1101101, //5
               0b1111101, //6
               0b0000111, //7
               0b1111111, //8
               0b1101111};//9
  int 




}
