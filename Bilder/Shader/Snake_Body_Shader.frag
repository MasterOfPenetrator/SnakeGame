#version 120
// Shader for Body Elements

// Input Texture
uniform sampler2D Body_Texture;

// Input Variables
uniform int Body_Health;
uniform bool Body_Death;
uniform float Time;

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
    // Getting Actual Tex Color
    vec4 Tex_Color = texture2D(Body_Texture, gl_TexCoord[0].xy);

    // Define Color Mask
    vec4 Color_Mask = vec4(1.0f, -0.75f, -0.75f, 0.01f);

    // Update Alpha-Value from Color Mask
    if(Body_Health >= 0 && Body_Health <= 100)
    {
        Color_Mask.a += (100.0f - float(Body_Health)) * 0.01f;
    }

    // Update Texture Color
    if(Tex_Color.a > 0.4f) // 0.4f Experimental Value for ignoring transperancy fields in tex
    {
        Tex_Color.rgb += Color_Mask.rgb * Color_Mask.a;
    }

    // Setup FBM if Health is very low
    if(Body_Health >= 0.0f && Body_Health < 20.0f)
    {
        Tex_Color *= FBM(Time);
    }

    // Make Cool Effect, if Snake is Dead
    if(Body_Death)
    {
        Tex_Color *= 0.0f;
    }

    // Setup our Pixel Color
	gl_FragColor = Tex_Color;
}
