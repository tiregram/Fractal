typedef double2 Complex;

void kernel frac(global const float* A,
                       global const int* B,
                       global int* C)
{

  int height = B[0];
  int width =  B[1];

  float x = A[2];
  float y = A[3];

  int zoom = A[4];

  double x_image = get_global_id(0) % (width);
  double y_image = get_global_id(0) / (width);

  Complex zn;
  Complex c;

  c.s0= A[0];
  c.s1= A[1];

  zn.s0 = ((x_image-width/2)/zoom) + x;
  zn.s1 = ((y_image-height/2)/zoom) + y;

  int i = 0;
    do
      {
      double tmp = zn.s0;
      zn.s0 = zn.s0 * zn.s0 - zn.s1 * zn.s1+c.s0 ;
      zn.s1= 2.0 * tmp * zn.s1 + c.s1;
      i++;
      }
    while( (zn.s0 * zn.s0) + (zn.s1 * zn.s1) < 4.0 &&
           i<126
           );

    float val = ((float)(126-i))/126;

    int R = 256 * val* val;
    int G = 256 * val * val* val ;
    int Bl =256 * val ;
    C[get_global_id(0)] = (Bl<<16) +(G << 8) + R ;
}
