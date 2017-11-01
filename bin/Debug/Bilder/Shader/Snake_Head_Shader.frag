#version 120
// Shader for Snake Head
// Function 1: Displaying Health with red Color Overlay
// Function 2: Rotate the texture

// Input Texture
uniform sampler2D Head_Texture;

// Input Variables
uniform vec2 Head_Resolution;
uniform int Head_Rotate_Angle;
uniform int Head_Health;

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
    float Angle = radians(Head_Rotate_Angle);

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

    // Setup our Pixel Color
	gl_FragColor = Tex_Color;
}
