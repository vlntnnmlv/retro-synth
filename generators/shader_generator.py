from string import Template

CLASS_NAME = "ShaderParameterSetter"

HEADER_CONTENT = \
"""#pragma once

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/glu.h>

class %(name)s
{
    private:
        GLuint m_gl_program_id;

    public:
        %(name)s();
        %(name)s(GLuint programm_id);
        ~%(name)s();
""" % {"name" : CLASS_NAME }

setter_function_definition_template = Template( \
"""
        void set_$value_format(const char *parameter_name, $params_amount_parameter$parameter_names_with_types);
""" \
)

SRC_CONTENT = \
"""#include "%(name)s.h"

%(name)s::%(name)s()
{

}

%(name)s::%(name)s(GLuint programm_id) : m_gl_program_id(programm_id)
{

}

%(name)s::~%(name)s()
{

}
""" % {"name" : CLASS_NAME }


setter_function_template = Template( \
"""
void %(name)s::set_$value_format(const char *parameter_name, $params_amount_parameter$parameter_names_with_types)
{
    glUniform$value_format(glGetUniformLocation(m_gl_program_id, parameter_name), $params_amount$parameter_values);
}\n
""" % {"name" : CLASS_NAME } \
)

def generate_types_data():
    data_types = ["f", "i", "ui"]
    data_pointer_types = ["fv", "iv", "uiv"]
    params_amount = [1, 2, 3, 4]

    def data_type_to_type_prefix(data_type):
        match (data_type):
            case "f":
                return "GLfloat "
            case "i":
                return "GLint "
            case "ui":
                return "GLuint "
            case "fv":
                return "const GLfloat *"
            case "iv":
                return "const GLint *"
            case "uiv":
                return "const GLuint *"

    data = {}
    for dt in data_types:
        for n in params_amount:
           data[f"{n}{dt}"] = {
               "value_format"                : f"{n}{dt}",
               "parameter_names_with_types"  : ", ".join([f"{data_type_to_type_prefix(dt)}{dt}{i}" for i in range(1, n + 1)]),
               "parameter_values"            : ", ".join([f"{dt}{i}" for i in range(1, n + 1)]),
               "params_amount"               : "",
               "params_amount_parameter"     : ""
           }

    for dt in data_pointer_types:
        for n in params_amount:
           data[f"{n}{dt}"] = {
               "value_format"                : f"{n}{dt}",
               "parameter_names_with_types"  : f"{data_type_to_type_prefix(dt)}{dt}{n}",
               "parameter_values"            : f"{dt}{n}",
               "params_amount"               : "count, ",
               "params_amount_parameter"     : "int count, "
           }

    return data
    

data = generate_types_data()

header = open(f"./include/{CLASS_NAME}.h", 'w')
header.write(HEADER_CONTENT)

src = open(f"./src/{CLASS_NAME}.cpp", 'w')
src.write(SRC_CONTENT)

for parameter_format in data.keys():
    header.write("  ");
    src.write("  ");

    function_defintion = setter_function_definition_template.substitute(data[parameter_format])
    header.write(function_defintion)

    function_realization = setter_function_template.substitute(data[parameter_format])
    src.write(function_realization)

header.write("};\n")

header.close()
src.close()
