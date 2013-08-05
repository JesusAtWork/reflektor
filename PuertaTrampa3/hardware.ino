void display_show_energia(int valor) {
    char str_energia[10];
    snprintf(str_energia, 6, "%05d", valor);
    display.show(str_energia);
}
