#version 330 core                                                                   
out vec4 FragColor; 

uniform int num;
uniform vec2 points[512]; 
uniform vec2 res;

float r = 1; //2.2;
float a = 0.1; 
float col= 0.1;  
float pow  = 1; //0.8                                                                                
void main(){                                                                        

	for(int i = 0; i < num; i++){
		col = col+ a*(r/(pow(distance(gl_FragCoord.xy/res, (points[i]+ 0.9)*0.6 ), pow)))/num;

	}
    col = 1.0 - col;
    float r = (sin(col*3.14159)+1)*0.5;
    float g = (cos(col*3.14159)+1)*0.5;
    float b = (asin(col*3.14159)+1)*0.5;
	// FragColor = vec4(1.0-r, r*0, g, 1);
	FragColor = vec4(1.0-r, g*0.3, b, 1);
  	// FragColor = vec4(col, col, col, 1);
                                            
}                                                                                   
         