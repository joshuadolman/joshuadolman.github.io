#ifndef HTML_TEMPLATES_H
#define HTML_TEMPLATES_H

char* SECTION_PAGE = "\
<!DOCTYPE html>\n<html>\n\
<head>\n\
\t<link rel=\"stylesheet\" href=\"%s\">\n\
\t<title>%s</title>\n\
</head>\n\
\t<a href=\"index.html\">\n\
\t<h1>Joshua Dolman</h1>\n\
</a>\n<hr>\n";
// [1] style.css filename and path + version string (../style.css?version=1)
// [2] tab title

char* CSS_STRING = "%sstyle.css?version=%d";

#endif