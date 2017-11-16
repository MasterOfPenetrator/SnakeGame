#version 120
// Shader for Snake Head
// Function 1: Displaying Health with red Color Overlay
// Function 2: Rotate the texture

// Input Texture
uniform sampler2D Head_Texture;

// Constants
const float Desaturate_Factor = 1.0f;

// Input Variables
uniform vec2 Head_Resolution;
uniform int Head_Rotate_Angle;
uniform int Head_Health;
uniform bool Head_Death;
uniform float Time;
uniform bool Desaturate;

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
        Frequency *= 2.0f;
        Amplitude *= 0.5f;
    }

    return Value;
}

// Main Function
void main()
{
    // Define red Color
    vec4 Color_Mask = vec4(1.0f, -0.75f, -0.75f, 0.01f);

    // Update Alpha-Value from Color Mask
    if(Head_Health >= 0 && Head_Health <= 100)
    {
        Color_Mask.a += (100.0f - float(Head_Health)) * 0.01f;
    }

    // Convert Angle to Radians
    float Angle = radians(float(Head_Rotate_Angle));

    // Getting Texture Coordinates and correct for Aspect Ratio of our Texture Resolution
    // Setting the new origin to center of your Picture vec2 - 0.5
    vec2 TexCoord = vec2(gl_TexCoord[0].x - (0.5f * Head_Resolution.x / Head_Resolution.y), gl_TexCoord[0].y -0.5f);

    // Setting Rotation Matrix
    mat2 RotateMat = mat2(cos(Angle), -sin(Angle), sin(Angle), cos(Angle));

    // Multiply
    TexCoord *= RotateMat;

    // Move to the origin position
    TexCoord += 0.5f;

    // Getting Actual Tex Color
    vec4 Tex_Color = texture2D(Head_Texture, TexCoord);

    // Update Texture Color
    if(Tex_Color.a > 0.4f) // 0.4f Experimental Value for ignoring transperancy fields in tex
    {
        Tex_Color.rgb += Color_Mask.rgb * Color_Mask.a;
    }

    // Setup FBM if Health is very low
    if(Head_Health >= 0.0f && Head_Health < 20.0f)
    {
        Tex_Color *= FBM(Time);
    }

    // Make Cool Effect, if Snake is Dead
    if(Head_Death)
    {
        Tex_Color *= 0.0f;
    }

    // Desature
    if(Desaturate)
    {
        float Intensity = (0.3f * Tex_Color.r) + (0.59f * Tex_Color.g) + (0.11f * Tex_Color.b);

        Tex_Color.r = (Intensity * Desaturate_Factor) + (Tex_Color.r * (1.0f - Desaturate_Factor));
        Tex_Color.g = (Intensity * Desaturate_Factor) + (Tex_Color.g * (1.0f - Desaturate_Factor));
        Tex_Color.b = (Intensity * Desaturate_Factor) + (Tex_Color.b * (1.0f - Desaturate_Factor));
    }

    // Setup our Pixel Color
	gl_FragColor = Tex_Color;
}
