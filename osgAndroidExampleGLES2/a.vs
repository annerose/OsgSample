#ifdef GL_FRAGMENT_PRECISION_HIGH 
   precision highp float; 
#else 
   precision mediump float; 
#endif

varying vec4 color;                                                    
const vec3 lightPos      =vec3(0.0, 0.0, 10.0);                        
const vec4 cessnaColor   =vec4(0.8, 0.8, 0.8, 1.0);                    
const vec4 lightAmbient  =vec4(0.1, 0.1, 0.1, 1.0);                    
const vec4 lightDiffuse  =vec4(0.4, 0.4, 0.4, 1.0);                    
const vec4 lightSpecular =vec4(0.8, 0.8, 0.8, 1.0);                    
varying vec2 v_texCoord;                                         
varying vec4 kOutBaseTCoord; 

const float EPSILON = 0.00001;

void DirectionalLight(in vec3 normal,                                  
				  in vec3 ecPos,                                   
				  inout vec4 ambient,                              
				  inout vec4 diffuse,                              
				  inout vec4 specular)                             
{                                                                      
	float nDotVP;                                                     
	vec3 L = normalize(gl_ModelViewMatrix*vec4(lightPos, 0.0)).xyz;   
	nDotVP = max(0.0, dot(normal, L));                                
																   
	if (nDotVP > 0.0) {                                               
		vec3 E = normalize(-ecPos);                                     
		vec3 R = normalize(reflect( L, normal ));                       
		specular = pow(max(dot(R, E), 0.0), 16.0) * lightSpecular;      
	}                                                                 
	ambient  = lightAmbient;                                          
	diffuse  = lightDiffuse * nDotVP;                                 
}                                                                      

void fswap(inout float a, inout float b)                                 
{                                 
	float thetemp = a;                                 
	a = b;                                 
	b = thetemp;
}

 

float getCofactor(float m0, float m1, float m2,
                           float m3, float m4, float m5,
                           float m6, float m7, float m8)
{
    return m0 * (m4 * m8 - m5 * m7) -
           m1 * (m3 * m8 - m5 * m6) +
           m2 * (m3 * m7 - m4 * m6);
}


void invert3(inout float m[9])
{
    float determinant, invDeterminant;
    float tmp[9];

    tmp[0] = m[4] * m[8] - m[5] * m[7];
    tmp[1] = m[2] * m[7] - m[1] * m[8];
    tmp[2] = m[1] * m[5] - m[2] * m[4];
    tmp[3] = m[5] * m[6] - m[3] * m[8];
    tmp[4] = m[0] * m[8] - m[2] * m[6];
    tmp[5] = m[2] * m[3] - m[0] * m[5];
    tmp[6] = m[3] * m[7] - m[4] * m[6];
    tmp[7] = m[1] * m[6] - m[0] * m[7];
    tmp[8] = m[0] * m[4] - m[1] * m[3];

    // check determinant if it is 0
    determinant = m[0] * tmp[0] + m[1] * tmp[3] + m[2] * tmp[6];
    if(abs(determinant) <= EPSILON)
    {
	    m[0] = m[4] = m[8] = 1.0;
		m[1] = m[2] = m[3] = m[5] = m[6] = m[7] = 0.0;
        
    }
	else
	{

		// divide by the determinant
		invDeterminant = 1.0 / determinant;
		m[0] = invDeterminant * tmp[0];
		m[1] = invDeterminant * tmp[1];
		m[2] = invDeterminant * tmp[2];
		m[3] = invDeterminant * tmp[3];
		m[4] = invDeterminant * tmp[4];
		m[5] = invDeterminant * tmp[5];
		m[6] = invDeterminant * tmp[6];
		m[7] = invDeterminant * tmp[7];
		m[8] = invDeterminant * tmp[8];
	}
 
}


void invertAffine4(inout float m[16] )
{
    // R^-1
    float r[9];
 	r[0] = m[0];
	r[1] = m[1];
	r[2] = m[2];
	r[3] = m[4];
	r[4] = m[5];
	r[5] = m[6];
	r[6] = m[8];
	r[7] = m[9];
	r[8] = m[10];
 
    invert3(r);
	
    m[0] = r[0];  m[1] = r[1];  m[2] = r[2];
    m[4] = r[3];  m[5] = r[4];  m[6] = r[5];
    m[8] = r[6];  m[9] = r[7];  m[10]= r[8];

    // -R^-1 * T
    float x = m[12];
    float y = m[13];
    float z = m[14];
    m[12] = -(r[0] * x + r[3] * y + r[6] * z);
    m[13] = -(r[1] * x + r[4] * y + r[7] * z);
    m[14] = -(r[2] * x + r[5] * y + r[8] * z);

    // last row should be unchanged (0,0,0,1)
    m[3] = m[7] = m[11] = 0.0;
    m[15] = 1.0;
 
}
 

///////////////////////////////////////////////////////////////////////////////
// compute the inverse of a general 4x4 matrix using Cramer's Rule
// If cannot find inverse, return indentity matrix
// M^-1 = adj(M) / det(M)
///////////////////////////////////////////////////////////////////////////////
void invertGeneral4(inout float m[16] )
{

  
    // get cofactors of minor matrices
    float cofactor0 = getCofactor(m[5],m[6],m[7], m[9],m[10],m[11], m[13],m[14],m[15]);
    float cofactor1 = getCofactor(m[4],m[6],m[7], m[8],m[10],m[11], m[12],m[14],m[15]);
    float cofactor2 = getCofactor(m[4],m[5],m[7], m[8],m[9], m[11], m[12],m[13],m[15]);
    float cofactor3 = getCofactor(m[4],m[5],m[6], m[8],m[9], m[10], m[12],m[13],m[14]);

    // get determinant
    float determinant = m[0] * cofactor0 - m[1] * cofactor1 + m[2] * cofactor2 - m[3] * cofactor3;
 //    if(abs(determinant) <= EPSILON)
 //    {
	// 	//	identity4
	// 	m[0] = m[5] = m[10] = m[15] = 1.0f;
	// 	m[1] = m[2] = m[3] = m[4] = m[6] = m[7] = m[8] = m[9] = m[11] = m[12] = m[13] = m[14] = 0.0f;

       
 //    }
	// else
	{

		// get rest of cofactors for adj(M)
		float cofactor4 = getCofactor(m[1],m[2],m[3], m[9],m[10],m[11], m[13],m[14],m[15]);
		float cofactor5 = getCofactor(m[0],m[2],m[3], m[8],m[10],m[11], m[12],m[14],m[15]);
		float cofactor6 = getCofactor(m[0],m[1],m[3], m[8],m[9], m[11], m[12],m[13],m[15]);
		float cofactor7 = getCofactor(m[0],m[1],m[2], m[8],m[9], m[10], m[12],m[13],m[14]);

		float cofactor8 = getCofactor(m[1],m[2],m[3], m[5],m[6], m[7],  m[13],m[14],m[15]);
		float cofactor9 = getCofactor(m[0],m[2],m[3], m[4],m[6], m[7],  m[12],m[14],m[15]);
		float cofactor10= getCofactor(m[0],m[1],m[3], m[4],m[5], m[7],  m[12],m[13],m[15]);
		float cofactor11= getCofactor(m[0],m[1],m[2], m[4],m[5], m[6],  m[12],m[13],m[14]);

		float cofactor12= getCofactor(m[1],m[2],m[3], m[5],m[6], m[7],  m[9], m[10],m[11]);
		float cofactor13= getCofactor(m[0],m[2],m[3], m[4],m[6], m[7],  m[8], m[10],m[11]);
		float cofactor14= getCofactor(m[0],m[1],m[3], m[4],m[5], m[7],  m[8], m[9], m[11]);
		float cofactor15= getCofactor(m[0],m[1],m[2], m[4],m[5], m[6],  m[8], m[9], m[10]);

		// build inverse matrix = adj(M) / det(M)
		// adjugate of M is the transpose of the cofactor matrix of M
		float invDeterminant = 1.0 / determinant;
		m[0] =  invDeterminant * cofactor0;
		m[1] = -invDeterminant * cofactor4;
		m[2] =  invDeterminant * cofactor8;
		m[3] = -invDeterminant * cofactor12;

		m[4] = -invDeterminant * cofactor1;
		m[5] =  invDeterminant * cofactor5;
		m[6] = -invDeterminant * cofactor9;
		m[7] =  invDeterminant * cofactor13;

		m[8] =  invDeterminant * cofactor2;
		m[9] = -invDeterminant * cofactor6;
		m[10]=  invDeterminant * cofactor10;
		m[11]= -invDeterminant * cofactor14;

		m[12]= -invDeterminant * cofactor3;
		m[13]=  invDeterminant * cofactor7;
		m[14]= -invDeterminant * cofactor11;
		m[15]=  invDeterminant * cofactor15;
	
	}
	
	 
}

void invert4(in mat4 matSrc, out mat4 matDst)
{

//	matDst = matSrc;
//	return;
 
	
	int i,j,k = 0;
	float m[16];
	
 
	for(i = 0; i < 4; i++)
	{

		m[i*4 ] 	= matSrc[i][0];
		m[i*4 + 1] 	= matSrc[i][1];
		m[i*4 + 2] 	= matSrc[i][2];
		m[i*4 + 3] 	= matSrc[i][3];
  
	}
	
 
 

    // If the 4th row is [0,0,0,1] then it is affine matrix and
    // it has no projective transformation.
    //if(m[3] == 0.0 && m[7] == 0.0 && m[11] == 0.0 && m[15] == 1.0)
	// if( (abs(m[3]) <= EPSILON)  
	// 	&& (abs(m[7]) <= EPSILON) 
	// 	&& (abs(m[11]) <= EPSILON) 
	// 	&& (abs(m[15] - 1.0) <= EPSILON) )
 
	// {
	//     //invertAffine4(m);
 // 		// invertGeneral4(m);

	// }
    // else
    // {
   	     invertGeneral4(m);
 
    // }
 

		
	for(i = 0; i < 4; i++)
	{

		matDst[i][0] = m[i*4 ];
		matDst[i][1] = m[i*4 + 1];
		matDst[i][2] = m[i*4 + 2];
		matDst[i][3] = m[i*4 + 3];
  
	}


  
}

void main() 
{                                                          
	vec4 ambiCol = vec4(0.0);                                          
	vec4 diffCol = vec4(0.0);                                          
	vec4 specCol = vec4(0.0);                                          
	gl_Position   = gl_ModelViewProjectionMatrix * gl_Vertex;          
	vec3 normal   = normalize(gl_NormalMatrix * gl_Normal);            
	vec4 ecPos    = gl_ModelViewMatrix * gl_Vertex;                    
	DirectionalLight(normal, ecPos.xyz, ambiCol, diffCol, specCol);    
	color = cessnaColor * (ambiCol + diffCol + specCol);               
	v_texCoord   = gl_MultiTexCoord0.xy;  
	
	vec4 sPlane = vec4(1.0, 0.0, 0.0, 0.0);                         
	vec4 tPlane = vec4(0.0, 1.0, 0.0, 0.0);                         
//	     vec4 rPlane = vec4(0.0, 0.0, 0.0, 0.0);                         
//		  vec4 qPlane = vec4(0.0, 0.0, 0.0, 0.0);                         
	kOutBaseTCoord.s = dot(vec4(gl_Vertex.xyz, 1.0), sPlane);                         
	kOutBaseTCoord.t = dot(vec4(gl_Vertex.xyz, 1.0), tPlane);     
  
	return;                    



	//	https://www.opengl.org/wiki/Mathematics_of_glTexGen
	//	Calculating for GL_SPHERE_MAP on the CPU
	vec4 myEyeVertex = normalize(gl_ModelViewMatrix * gl_Vertex);                       

//		vec4 myEyeNormal =  vec4 (gl_Normal, 1)  *    inverse(gl_ModelViewMatrix) ;                       
// 	vec4 myEyeNormal =  vec4 (gl_Normal, 1)  *   gl_ModelViewMatrixInverse ;                       

	mat4 mModelViewMatrixInverse;														
	invert4 (gl_ModelViewMatrix, mModelViewMatrixInverse);							
	vec4 myEyeNormal =  vec4 (gl_Normal, 1)  *  mModelViewMatrixInverse; 

	vec4 reflectionVector = myEyeVertex - myEyeNormal * 2.0 * dot(myEyeVertex, myEyeNormal);                       
	reflectionVector.z += 1.0;                       
	float m = 1.0 / (2.0 * sqrt(dot(reflectionVector, reflectionVector)));                       
	kOutBaseTCoord.s = reflectionVector.x * m + 0.5;                       
	kOutBaseTCoord.t = reflectionVector.y * m + 0.5;    


	//	Calculating for GL_EYE_LINEAR on the CPU
	
//	vec4 myEyePlane_S = sPlane *  gl_ModelViewMatrix;
//	vec4 myEyePlane_T = tPlane *  gl_ModelViewMatrix;
	
//	vec4 myEyeVertex = gl_ModelViewMatrix * gl_Vertex;  	
//	kOutBaseTCoord.s = dot(myEyeVertex, myEyePlane_S); 
//	kOutBaseTCoord.t = dot(myEyeVertex, myEyePlane_T); 
}       