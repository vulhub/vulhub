#!/usr/local/bash-4.3.0/bin/bash
echo "Content-type: text/html"
echo ""
echo '<html>'
echo '<head>'
echo '<meta http-equiv="Content-Type" content="text/html; charset=utf-8">'
echo '<title>Bash ShellShock</title>'
echo '</head>'
echo '<body>'
echo '<pre>'
/usr/bin/env
echo '</pre>'
echo '</body>'
echo '</html>'
exit 0