#version 120
// Shader for Simple Pistol, Weapon

// Input Texture
uniform sampler2D Texture;

// Input Variables
uniform float Time;
uniform int Rotation;
uniform vec2 Resolution;

// Functions
// Noise
float Noise(float x)
{
	float i = floor(x);
	float f = fract(x);
	float u = f * f * (3.0f - 2.0f * f);
	return mix(fract(sin(i) * 43758.5453123f), fract(sin(i + 1.0f) * 43758.5453123f), u);
}
// Fractional Brown Motion
float FBM(float x)
{
	float Value = 0.0f;
	float Amplitude = 0.75f;
	float Frequency = 2.0f;
	for(int i = 0; i<10; i++)
    {
        Value += Noise(x * Frequency) * Amplitude;
        Frequency *= 1.25f;
        Amplitude *= 0.3f;
    }

    return Value;
}

// Main Function
void main()
{
    // Convert Angle to Radians
    float Angle = radians(float(Rotation));

    // Getting Texture Coordinates and correct for Aspect Ratio of our Texture Resolution
    // Setting the new origin to center of your Picture vec2 - 0.5
    vec2 TexCoord = vec2(gl_TexCoord[0].x - (0.5f * Resolution.x / Resolution.y), gl_TexCoord[0].y -0.5f);

    // Setting Rotation Matrix
    mat2 RotateMat = mat2(cos(Angle), -sin(Angle), sin(Angle), cos(Angle));

    // Multiply
    TexCoord *= RotateMat;

    // Move to the origin position
    TexCoord += 0.5f;

    // Output the Color
    gl_FragColor = texture2D(Texture, TexCoord);
}
