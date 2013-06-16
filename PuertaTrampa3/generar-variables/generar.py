from variables import variables
DEBUG=True

if __name__ == "__main__":
    if DEBUG:
        print "#define DEBUG"
        print
        for (nombre, valor_default, (_, _)) in variables:
            print "int %s = %d;" % (nombre, valor_default)
        print
        print "int* variables[] = {"
        for (nombre, _, (_, _)) in variables:
            print "    &%s," % nombre
        print "    0"
        print "};"
    else:
        for (nombre, valor_default, (_, _)) in variables:
            print "#define %s %d" % (nombre, valor_default)
