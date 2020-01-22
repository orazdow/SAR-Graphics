#version 330 core                                                                   
out vec4 FragColor;                                                                 
                                                                                
void main(){                                                                        
	FragColor = vec4(0.0, 0.7, 1, 0.8-pow(distance(gl_PointCoord, vec2(0.9, 0.1)), 0.5) );
// FragColor = vec4(0,0,0,1.0);                                                  
}                                                                                   
         