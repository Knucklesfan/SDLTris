#include "ttffont.h"
#include "utils/defs.h"
#include <freetype2/freetype/freetype.h>

ttffont::ttffont(std::string path) { 
    projection = glm::ortho(0.0f, static_cast<float>(640), 0.0f, static_cast<float>(480)); //THIS WAS THE STUPID BUG ALL ALONG???
    //Why does this work? I don't know.
    //What makes this work? I don't know.
    //Who made this work? I don't know.
    //I have so many questions, and none of them make sense....
    //god im tired
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    //this code is uhhh **borrowed** from learnopengl... Sorry guys... :(
    std::string p = "./fonts/" + path + "/";

    std::string pth = filepath "fonts/" + path + "/fontdef.xml";

    rapidxml::file<> xmlFile(pth.c_str());
    rapidxml::xml_document<> doc;
    doc.parse<0>(xmlFile.data());
    shad = graphics::shaders.at(atoi(doc.first_node("shader")->value())); //specifies the shader slot number to pick from the shaders
    std::string fullpath = p + (doc.first_node("filename")->value()); //specifies the filename of the font
    int maxsize = (atoi(doc.first_node("maxsize")->value())); //specifies the font's maximum size

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        exit(-1);
    }

    FT_Face face;
    if (FT_New_Face(ft, fullpath.c_str(), 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font " << fullpath  << std::endl;  
        exit(-1);
    }
    FT_Set_Pixel_Sizes(face, 0, maxsize);  
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        ttfchar character = {
            texture, 
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        characters.insert(std::pair<char, ttfchar>(c, character));
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

// int error;
// error = FT_Init_FreeType(&ft);
// if (error) {
//     printf("ERROR CODE: %d", error);
// }
// error = FT_New_Face(ft, "/home/knucklesfan/Documents/GitHub/SDLTris/fonts/freesans/arial.ttf", 0, &face);
// if (error) {
//     printf("ERROR CODE: %d", error);
// }
// error = FT_Set_Pixel_Sizes(face, 100, 100);
// if (error) {
//     printf("ERROR CODE: %d", error);
// }
// error = FT_Load_Glyph(face, 'x', FT_LOAD_DEFAULT);
// if (error) {
//     printf("ERROR CODE: %d", error);
// }
// error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);

// if (error) {
//     printf("ERROR CODE: %d", error);
// }
}

void ttffont::render(int x, int y, std::string strg, bool center) {
    render(strg, x, y, center, 0, 0, 0);
}
void ttffont::render(int x, int y, std::string strg, bool center, bool sine, double pos, double multiplyin, double multiplyout) {
    render(strg, x, y, center, 0, 0, 0, 0, sine, pos, multiplyin, multiplyout, 1.0);
}

void ttffont::render(int x, int y, std::string strg, bool center, int wordwrap) {
    render(strg, x, y, center, 0, 0, 0, wordwrap);
}
void ttffont::render(std::string words, int x, int y, bool center, int red, int blue, int green) {
    render(words, x, y, center, red, blue, green, 0);
}
void ttffont::render(std::string strg, int x, int y, bool center) {
    render(strg, x, y, center, 0, 0, 0);
}
void ttffont::render(std::string words, int x, int y, bool center, int red, int blue, int green, int wordwrap) {
    render(words, x, y, center, red, blue, green, wordwrap, false, 0, 0, 0, 1.0);
}
void ttffont::render(std::string words, int x, int y, bool center, int red, int blue, int green, int wordwrap, bool sine, double pos, double multiplyin, double multiplyout, double scale) {

    shad->activate();
    glm::vec3 color = (glm::vec3(red/255.0f,blue/255.0f,green/255.0f));
    shad->setVec3("textColor",glm::value_ptr(color));
    glUniformMatrix4fv(glGetUniformLocation(shad->id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    // glUniform3f(glGetUniformLocation(s.Program, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    // iterate through all characters
    std::string::const_iterator c;
    for (c = words.begin(); c != words.end(); c++)
    {
        ttfchar ch = characters[*c];
        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }           
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.textureid);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}