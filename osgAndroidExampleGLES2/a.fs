#ifdef GL_FRAGMENT_PRECISION_HIGH 
   precision highp float; 
#else 
   precision mediump float; 
#endif
                
varying mediump vec4 color;               
uniform sampler2D sam;                                 

varying  mediump vec2 v_texCoord;                      
varying mediump vec4 kOutBaseTCoord;  
void main() {                             
//      gl_FragColor = color;                   
	    gl_FragColor = texture2D(sam, v_texCoord)  * (color + 0.25);          
//		gl_FragColor = clamp(color, 0.0, 1.0);                   
		// gl_FragColor = texture2D(sam, kOutBaseTCoord.st) * (color + 0.25); 
	  // gl_FragColor = texture2D(sam, kOutBaseTCoord.st);
}     