typedef double2 Complex;

#define mandel 0
#define julia 1

void kernel frac(global const float* A,
                 global const int* B,
                 global int* C)
{

  int height = B[0];
  int width =  B[1];
  int type =  B[2];//0)mandel-1)julia

  float x = A[2];
  float y = A[3];

  int zoom = A[4];

  double x_image = get_global_id(0) % (width);
  double y_image = get_global_id(0) / (width);

  Complex zn;
  Complex c;

    if(type==0){
      c.s0=    ((x_image-width/2)/zoom) + x;
      c.s1=     ((y_image-height/2)/zoom) + y;

      zn.s0 = 0;//A[0];
      zn.s1 =0;//A[1];
    }else//julia
      {
        c.s0= A[0];
        c.s1= A[1];

        zn.s0 = ((x_image-width/2)/zoom) + x;
        zn.s1 = ((y_image-height/2)/zoom) + y;
      }


  int i = 0;
    do
      {
      double tmp = zn.s0;
      zn.s0 = zn.s0 * zn.s0 - zn.s1 * zn.s1+c.s0 ;
      zn.s1= 2.0 * tmp * zn.s1 + c.s1;
      i++;
      }
    while( (zn.s0 * zn.s0) + (zn.s1 * zn.s1) < 4.0 &&
           i<256
           );

    float val = ((float)(256-i))/256;

    int R = 256 * val * val;
    int G = 256 * val * val * val ;
    int Bl =256 * sqrt(val) ;
    C[get_global_id(0)] = (Bl<<16) +(G << 8) + R ;
}
