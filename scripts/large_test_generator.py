import random
import copy

used_name = dict()
consts = dict()
procedures = dict()
functions = dict()
now_function_name = ""
now_function_return_type = 0
now_function_parameter = []
for i in range(0, 4):
    consts[i] = []
vars = dict()
for i in range(0, 4):
    vars[i] = []
file = open("input.pas", "w")

def random_string(length):
    length = random.randint(1, length)
    letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_"
    return ''.join(random.choice(letters) for i in range(length))

def random_name():
    name = random_string(10)
    while name[0] >= '0' and name[0] <= '9':
        name = random_string(10)
    return name

def generate_random_name():
    name = random_name()
    lower_name = name.lower()
    while lower_name in used_name:
        name = random_name()
        lower_name = name.lower()
    used_name[lower_name] = True
    return name

def generate_int_value():
    return random.randint(-100000, 100000)

def generate_real_value():
    return random.uniform(-100000, 100000)

def generate_char_value():
    char = random_string(1)
    if char == '_':
        return "'a'"
    else:
        return "'" + random_string(1) + "'"

def generate_bool_value():
    value = random.randint(0, 1)
    if value == 0:
        return "false"
    else:
        return "true"

def generate_random_value(value_type):
    if value_type == 0:
        return generate_int_value()
    elif value_type == 1:
        return generate_real_value()
    elif value_type == 2:
        return generate_char_value()
    elif value_type == 3:
        return generate_bool_value()

def generate_function_params(function_name, now_vars, now_consts):
    parameters = functions[function_name][1]
    parameter_num = len(parameters)
    #print("called! function name is: '" + function_name + "'\n")
    #print("parameter_num is: " + str(parameter_num) + "\n")
    if parameter_num == 0:
        return ""
    else:
        result = ""
        for i in range(0, parameter_num):
            if i != 0:
                result += ", "
            parameter_type = parameters[i][0]
            parameter_name = parameters[i][1]
            if parameter_type == 0:
                value_original_type = random.randint(0, 2)
                if value_original_type == 0:
                    value = generate_random_value(0)
                    result += str(value)
                elif value_original_type == 1:
                    value = random.choice(now_vars[0])
                    result += value
                else:
                    value = random.choice(now_consts[0])
                    result += value[0]
            elif parameter_type == 1:
                value_original_type = random.randint(0, 2)
                if value_original_type == 0:
                    value = generate_random_value(1)
                    result += str(value)
                elif value_original_type == 1:
                    value = random.choice(now_vars[1])
                    result += value
                else:
                    value = random.choice(now_consts[1])
                    result += value[0]
            elif parameter_type == 2:
                value_original_type = random.randint(0, 2)
                if value_original_type == 0:
                    value = generate_random_value(2)
                    result += str(value)
                elif value_original_type == 1:
                    value = random.choice(now_vars[2])
                    result += value
                else:
                    value = random.choice(now_consts[2])
                    result += value[0]
            elif parameter_type == 3:
                value_original_type = random.randint(0, 1)
                if value_original_type == 0:
                    value = generate_random_value(3)
                    result += str(value)
                elif value_original_type == 1:
                    value = random.choice(now_vars[3])
                    result += value
        #print("result is: " + result + "\n")
        return result

def generate_if_else_statement(depth, now_vars, now_consts, is_main, is_function):
    file.write("if ")
    expression_type = random.randint(0, 1)
    if expression_type == 0: # compare value
        value_type = random.randint(0, 3)
        name = random.choice(now_vars[value_type])
        compare_value = generate_random_value(value_type)
        file.write(name + " = " + str(compare_value) + " then begin\n")
    elif expression_type == 1: # boolean expression
        name = random.choice(now_vars[3])
        file.write(name + " then begin\n")
    generate_statements(depth + 1, now_vars, now_consts, is_main, is_function)
    file.write("end\nelse begin\n")
    generate_statements(depth + 1, now_vars, now_consts, is_main, is_function)
    file.write("end;\n")

def generate_case_statement(depth, now_vars, now_consts, is_main, is_function):
    file.write("case ")
    value_type = random.randint(0, 3)
    if(value_type == 1): # real can't be used in case
        value_type = 0
    name = random.choice(now_vars[value_type])
    file.write(name + " of\n")
    case_num = random.randint(1, 10)
    if value_type == 3 and case_num > 2:
        case_num = 2
    already_used = dict()
    for i in range(0, case_num):
        case_value = generate_random_value(value_type)
        while case_value in already_used:
            case_value = generate_random_value(value_type)
        file.write("    " + str(case_value) + " : \nbegin\n")
        generate_statements(depth + 1, now_vars, now_consts, is_main, is_function)
        if i == case_num - 1:
            file.write("    end\n")
        else :
            file.write("    end;\n")
    file.write("end;\n")

def generate_for_statement(depth, now_vars, now_consts, is_main, is_function):
    file.write("for ")
    name = random.choice(now_vars[0])
    file.write(name + " := ")
    value1 = generate_random_value(0)
    value2 = generate_random_value(0)
    if value1 > value2:
        file.write(str(value1) + " downto " + str(value2) + " do\n")
    else:
        file.write(str(value1) + " to " + str(value2) + " do\n")
    file.write("begin\n")
    generate_statements(depth + 1, now_vars, now_consts, is_main, is_function)
    file.write("end;\n")

def generate_while_statement(depth, now_vars, now_consts, is_main, is_function):
    file.write("while ")
    expression_type = random.randint(0, 1)
    if expression_type == 0: # compare value
        value_type = random.randint(0, 3)
        name = random.choice(now_vars[value_type])
        compare_value = generate_random_value(value_type)
        file.write(name + " = " + str(compare_value) + " do\n")
    elif expression_type == 1: # boolean expression
        name = random.choice(now_vars[3])
        file.write(name + " do\n")
    file.write("begin\n")
    generate_statements(depth + 1, now_vars, now_consts, is_main, is_function)
    file.write("end;\n")

def generate_assign_statement(depth, now_vars, now_consts, is_function):
    generated = False
    if is_function == True:
        very_cool_random = random.randint(0, 5)
        if very_cool_random == 0:
            #print("called! function name is: '" + now_function_name + "'\n")
            generated = True
            file.write(now_function_name + " := ")
            assign_type = random.randint(0, 2)
            if assign_type == 0: # assign value
                value = generate_random_value(now_function_return_type)
                file.write(str(value) + ";\n")
            elif assign_type == 1: # assign from var/const
                is_const = random.randint(0, 1)
                if is_const == 0 or now_function_return_type == 3:
                    name2 = random.choice(now_vars[now_function_return_type])
                    file.write(name2 + ";\n")
                else:
                    name2 = random.choice(now_consts[now_function_return_type])
                    file.write(name2[0] + ";\n")
            else: # assign from function
                if len(now_function_parameter) == 0:
                    file.write(now_function_name + ";\n")
                else: 
                    file.write(now_function_name + "(")
                    file.write(generate_function_params(now_function_name, now_vars, now_consts))
                    file.write(");\n")
    if generated == False:
        value_type = random.randint(0, 3)
        name = random.choice(now_vars[value_type])    
        assign_type = random.randint(0, 1)
        if assign_type == 0 or value_type == 3: # assign value
            value = generate_random_value(value_type)
            file.write(name + " := " + str(value) + ";\n")
        elif assign_type == 1:
            is_const = random.randint(0, 1)
            if is_const == 0:
                name2 = random.choice(now_vars[value_type])
                file.write(name + " := " + name2 + ";\n")
            else:
                name2 = random.choice(now_consts[value_type])
                file.write(name + " := " + name2[0] + ";\n")

def generate_procedure_call_statement(now_vars, now_consts):
    procedure_name = random.choice(list(procedures.keys()))
    parameters = procedures[procedure_name]
    parameter_num = len(parameters)
    if parameters == 0:
        file.write(procedure_name + ";\n")
    else:
        file.write(procedure_name + "(")
        for i in range(0, parameter_num):
            if i != 0:
                file.write(", ")
            parameter_type = parameters[i][0]
            parameter_name = parameters[i][1]
            if parameter_type == 0:
                value_original_type = random.randint(0, 2)
                if value_original_type == 0:
                    value = generate_random_value(0)
                    file.write(str(value))
                elif value_original_type == 1:
                    value = random.choice(now_vars[0])
                    file.write(value)
                else:
                    value = random.choice(now_consts[0])
                    file.write(value[0])
            elif parameter_type == 1:
                value_original_type = random.randint(0, 2)
                if value_original_type == 0:
                    value = generate_random_value(1)
                    file.write(str(value))
                elif value_original_type == 1:
                    value = random.choice(now_vars[1])
                    file.write(value)
                else:
                    value = random.choice(now_consts[1])
                    file.write(value[0])
            elif parameter_type == 2:
                value_original_type = random.randint(0, 2)
                if value_original_type == 0:
                    value = generate_random_value(2)
                    file.write(str(value))
                elif value_original_type == 1:
                    value = random.choice(now_vars[2])
                    file.write(value)
                else:
                    value = random.choice(now_consts[2])
                    file.write(value[0])
            elif parameter_type == 3:
                value_original_type = random.randint(0, 1)
                if value_original_type == 0:
                    value = generate_random_value(3)
                    file.write(str(value))
                elif value_original_type == 1:
                    value = random.choice(now_vars[3])
                    file.write(value)
        file.write(");\n")

def generate_statements(depth, now_vars, now_consts, is_main, is_function):
    for i in range(0, 10):
        if depth < 3 and random.randint(0, 1) == 0:
            expend_type = random.randint(0, 3)
            if expend_type == 0: # if else statement
                generate_if_else_statement(depth, now_vars, now_consts, is_main, is_function)
            elif expend_type == 1: # case statement
                generate_case_statement(depth, now_vars, now_consts, is_main, is_function)
            elif expend_type == 2: # for statement
                generate_for_statement(depth, now_vars, now_consts, is_main, is_function)
            elif expend_type == 3: # while statement
                generate_while_statement(depth, now_vars, now_consts, is_main, is_function)
        else:
            simple_statement_type = random.randint(0, 1)
            if is_main == True:
                if simple_statement_type == 0:
                    generate_assign_statement(depth, now_vars, now_consts, is_function)
                else:
                    generate_procedure_call_statement(now_vars, now_consts)
            else:
                generate_assign_statement(depth, now_vars, now_consts, is_function)

def generate_procedures():
    for i in range(0, 5):
        procedure_name = generate_random_name()
        parameter_num = random.randint(0, 5)
        tmp_vars = copy.deepcopy(vars)
        parameters = []
        for i in range(0, parameter_num):
            parameter_type = random.randint(0, 3)
            parameter_name = generate_random_name()
            parameters.append((parameter_type, parameter_name))
        procedures[procedure_name] = parameters
        if parameter_num == 0:
            file.write("procedure " + procedure_name + ";\n")
        else :
            file.write("procedure " + procedure_name + "(")
            for i in range(0, parameter_num):
                if i != 0:
                    file.write("; ")
                parameter_type = parameters[i][0]
                parameter_name = parameters[i][1]
                if parameter_type == 0:
                    file.write(parameter_name + " : integer")
                elif parameter_type == 1:
                    file.write(parameter_name + " : real")
                elif parameter_type == 2:
                    file.write(parameter_name + " : char")
                elif parameter_type == 3:
                    file.write(parameter_name + " : boolean")
                tmp_vars[parameter_type].append(parameter_name)
            # var decl
            file.write(");\n")
        file.write("var\n")
        for i in range(0, 3):
            name = generate_random_name()
            type_value = random.randint(0, 3)
            if type_value == 0: # integer
                file.write("    " + name + " : integer;\n")
            elif type_value == 1: # real
                file.write("    " + name + " : real;\n")
            elif type_value == 2: # char
                file.write("    " + name + " : char;\n")
            elif type_value == 3: # bool
                file.write("    " + name + " : boolean;\n")
            tmp_vars[type_value].append(name)
        file.write("begin\n")
        generate_statements(2, tmp_vars, consts, False, False)
        file.write("end;\n")

def generate_functions():
    global now_function_name
    global now_function_return_type
    global now_function_parameter
    for i in range(0, 5):
        function_name = generate_random_name()
        parameter_num = random.randint(0, 5)
        now_function_name = function_name
        #print("now_function_name modified, now is " + now_function_name + "\n")
        return_type = random.randint(0, 3)
        now_function_return_type = return_type
        parameters = []
        tmp_vars = copy.deepcopy(vars)
        for i in range(0, parameter_num):
            parameter_type = random.randint(0, 3)
            parameter_name = generate_random_name()
            parameters.append((parameter_type, parameter_name))
        functions[function_name] = (return_type, parameters)
        now_function_parameter = parameters
        if parameter_num == 0:
            file.write("function " + function_name + ":")
            if return_type == 0:
                file.write("integer;\n")
            elif return_type == 1:
                file.write("real;\n")
            elif return_type == 2:
                file.write("char;\n")
            elif return_type == 3:
                file.write("boolean;\n")
        else :
            file.write("function " + function_name + "(")
            for i in range(0, parameter_num):
                if i != 0:
                    file.write("; ")
                parameter_type = parameters[i][0]
                parameter_name = parameters[i][1]
                if parameter_type == 0:
                    file.write(parameter_name + " : integer")
                elif parameter_type == 1:
                    file.write(parameter_name + " : real")
                elif parameter_type == 2:
                    file.write(parameter_name + " : char")
                elif parameter_type == 3:
                    file.write(parameter_name + " : boolean")
                tmp_vars[parameter_type].append(parameter_name)
            file.write("):")
            if return_type == 0:
                file.write("integer;\n")
            elif return_type == 1:
                file.write("real;\n")
            elif return_type == 2:
                file.write("char;\n")
            elif return_type == 3:
                file.write("boolean;\n")
        file.write("var\n")
        for i in range(0, 3):
            name = generate_random_name()
            type_value = random.randint(0, 3)
            if type_value == 0: # integer
                file.write("    " + name + " : integer;\n")
            elif type_value == 1: # real
                file.write("    " + name + " : real;\n")
            elif type_value == 2: # char
                file.write("    " + name + " : char;\n")
            elif type_value == 3: # bool
                file.write("    " + name + " : boolean;\n")
            tmp_vars[type_value].append(name)
        file.write("begin\n")
        generate_statements(2, tmp_vars, consts, False, True)
        file.write("end;\n")


file.write("program test;\n")
# const declaration

file.write("const\n")
for i in range(0, 20):
    name = generate_random_name()
    type_value = random.randint(0, 2)
    if type_value == 0: # integer
        value = random.randint(-100000, 100000)
        file.write("    " + name + " = " + str(value) + ";\n")
    elif type_value == 1: # real
        value = random.uniform(-100000, 100000)
        file.write("    " + name + " = " + str(value) + ";\n")

    elif type_value == 2: # char
        value = random_string(1)
        file.write("    " + name + " = '" + value + "';\n")
    consts[type_value].append((name, value))
file.write("\n")

# var declaration
file.write("var\n")
for i in range(0, 20):
    name = generate_random_name()
    type_value = random.randint(0, 3)
    if type_value == 0: # integer
        file.write("    " + name + " : integer;\n")
    elif type_value == 1: # real
        file.write("    " + name + " : real;\n")
    elif type_value == 2: # char
        file.write("    " + name + " : char;\n")
    elif type_value == 3: # bool
        file.write("    " + name + " : boolean;\n")
    vars[type_value].append(name)

file.write("\n")

generate_procedures()
generate_functions()

# main process
file.write("begin\n")
generate_statements(0, vars, consts, True, False)
file.write("end.\n")
file.close()