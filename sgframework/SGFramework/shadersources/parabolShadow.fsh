in vec3 pos;


void main()
{
  if(length(pos.xy)>1.005) discard;      //set undifined parts
}


