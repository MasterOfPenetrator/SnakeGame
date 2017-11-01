#version 120
// Shader for Body Elements

// Input Texture
uniform sampler2D Body_Texture;

// Input Variables
uniform int Body_Health;

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

    // Setup our Pixel Color
	gl_FragColor = Tex_Color;
}
