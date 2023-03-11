char index_html1[]=
"<html>\n\
<head>\n\
<title>Presettable Analog Delay Editor</title>\n\
<style type=\"text/css\">\n\
<!--\n\
\n\
html {\n\
	-webkit-text-size-adjust: 100%;\n\
}\n\
\n\
body,div,dl,dt,dd,ul,ol,li,h1,h2,h3,h4,h5,h6,pre,form,input,p,blockquote,th,td {\n\
	margin:0;\n\
	padding:0;\n\
	font-family: Arial, Helvetica, sans-serif;\n\
	color: #222222;\n\
	line-height: 1.5em;\n\
	font-size:18px;\n\
}\n\
table {\n\
	border-collapse:collapse;\n\
	border-spacing:0;\n\
	font-size:18px;\n\
	margin-left:8px;\n\
	margin-right:8px;\n\
	margin-top:2px;\n\
	margin-bottom: 2px;\n\
}\n\
h1 {\n\
	margin: 0.5em 0 0 0;\n\
	padding: 2px;\n\
	color: #0068B7;\n\
	font-weight: bold;\n\
	border-bottom: #0068B7 6px solid;\n\
	clear: both;\n\
	font-size: 20px;\n\
}\n\
.btn--orange,\n\
a.btn--orange {\n\
  color: #fff;\n\
  background-color: #eb6100;\n\
}\n\
.btn--orange:hover,\n\
a.btn--orange:hover {\n\
  color: #fff;\n\
  background: #f56500;\n\
}\n\
\n\
a.btn--radius {\n\
   border-radius: 100vh;\n\
}\n\
\n\
-->\n\
</style>\n\
\n\
<meta name=\"viewport\" content=\"width=device-width\" />\n\
\n\
</head>\n\
<body>\n\
\n\
<div>\n\
<h1>Presettable Analog Delay Editor V2.1<br>\n\
</h1>\n\
</div>\n\
  \n\
<form action=\"http://nld.local/preset\" method=\"get\" >\n\
\n\
\n\
  <table width=\"95%\" border=\"1\" cellpadding=\"0\" cellspacing=\"0\">\n\
    <tr>\n\
      <td width=\"10%\" bgcolor=\"#66CC99\"><div align=\"center\"><strong>Preset</strong></div></td>\n\
      <td width=\"30%\" bgcolor=\"#66CC99\"><div align=\"center\"><strong>Effect</strong></div></td>\n\
      <td width=\"30%\" bgcolor=\"#66CC99\"><div align=\"center\"><strong>Feedback</strong></div></td>\n\
      <td width=\"30%\" bgcolor=\"#66CC99\"><div align=\"center\"><strong>Time</strong></div></td>\n\
    </tr>\n\
    <tr>\n\
      <td width=\"10%\"><div align=\"center\">0</div></td>\n\
      <td width=\"30%\"><label>\n\
        <div align=\"center\">\n\
          <input type=\"text\" name=\"e0\" id=\"e0\" style=\"width: 100%;\" maxlength=\"2\">\n\
          </div>\n\
      </label></td>\n\
      <td width=\"30%\"><label>\n\
        <div align=\"center\">\n\
          <input type=\"text\" name=\"f0\" id=\"f0\" style=\"width: 100%;\" maxlength=\"2\">\n\
          </div>\n\
      </label></td>\n\
      <td width=\"30%\"><label>\n\
        <div align=\"center\">\n\
          <input type=\"text\" name=\"t0\" id=\"t0\" style=\"width: 100%;\" maxlength=\"3\">\n\
          </div>\n\
      </label></td></td>\n\
    </tr>\n\
    <tr>\n\
      <td><div align=\"center\">1</div></td>\n\
      <td><label>\n\
        <div align=\"center\">\n\
          <input type=\"text\" name=\"e1\" id=\"e1\" style=\"width: 100%;\" maxlength=\"2\"  >\n\
          </div>\n\
      </label></td>\n\
      <td><label>\n\
        <div align=\"center\">\n\
          <input type=\"text\" name=\"f1\" id=\"f1\" style=\"width: 100%;\" maxlength=\"2\"  >\n\
          </div>\n\
      </label></td>\n\
      <td><label>\n\
        <div align=\"center\">\n\
          <input type=\"text\" name=\"t1\" id=\"t1\" style=\"width: 100%;\" maxlength=\"3\"  >\n\
          </div>\n\
      </label></td>\n\
      </tr>\n\
    <tr>\n\
      <td><div align=\"center\">2</div></td>\n\
      <td><label>\n\
        <div align=\"center\">\n\
          <input type=\"text\" name=\"e2\" id=\"e2\" style=\"width: 100%;\" maxlength=\"2\">\n\
          </div>\n\
      </label></td>\n\
      <td><label>\n\
        <div align=\"center\">\n\
          <input type=\"text\" name=\"f2\" id=\"f2\" style=\"width: 100%;\" maxlength=\"2\">\n\
          </div>\n\
      </label></td>\n\
      <td><label>\n\
        <div align=\"center\">\n\
          <input type=\"text\" name=\"t2\" id=\"t2\" style=\"width: 100%;\" maxlength=\"3\">\n\
          </div>\n\
      </label></td>\n\
      </tr>\n\
    <tr>\n\
      <td><div align=\"center\">3</div></td>\n\
      <td><label>\n\
        <div align=\"center\">\n\
          <input type=\"text\" name=\"e3\" id=\"e3\" style=\"width: 100%;\" maxlength=\"2\">\n\
          </div>\n\
      </label></td>\n\
      <td><label>\n\
        <div align=\"center\">\n\
          <input type=\"text\" name=\"f3\" id=\"f3\" style=\"width: 100%;\" maxlength=\"2\">\n\
          </div>\n\
      </label></td>\n\
      <td><label>\n\
        <div align=\"center\">\n\
          <input type=\"text\" name=\"t3\" id=\"t3\" style=\"width: 100%;\" maxlength=\"3\">\n\
          </div>\n\
      </label></td>\n\
      </tr>\n\
    <tr>\n\
      <td><div align=\"center\">4</div></td>\n\
      <td><label>\n\
        <div align=\"center\">\n\
          <input type=\"text\" name=\"e4\" id=\"e4\" style=\"width: 100%;\" maxlength=\"2\">\n\
          </div>\n\
      </label></td>\n\
      <td><label>\n\
        <div align=\"center\">\n\
          <input type=\"text\" name=\"f4\" id=\"f4\" style=\"width: 100%;\" maxlength=\"2\">\n\
          </div>\n\
      </label></td>\n\
      <td><label>\n\
        <div align=\"center\">\n\
          <input type=\"text\" name=\"t4\" id=\"t4\" style=\"width: 100%;\" maxlength=\"3\">\n\
          </div>\n\
      </label></td>\n\
      </tr>\n\
    <tr>\n\
      <td><div align=\"center\">5</div></td>\n\
      <td><label>\n\
        <div align=\"center\">\n\
          <input type=\"text\" name=\"e5\" id=\"e5\" style=\"width: 100%;\" maxlength=\"2\">\n\
          </div>\n\
      </label></td>\n\
      <td><label>\n\
        <div align=\"center\">\n\
          <input type=\"text\" name=\"f5\" id=\"f5\" style=\"width: 100%;\" maxlength=\"2\">\n\
          </div>\n\
      </label></td>\n\
      <td><label>\n\
        <div align=\"center\">\n\
          <input type=\"text\" name=\"t5\" id=\"t5\" style=\"width: 100%;\" maxlength=\"3\">\n\
          </div>\n\
      </label></td>\n\
      </tr>\n\
    <tr>\n\
      <td><div align=\"center\">6</div></td>\n\
      <td><label>\n\
        <div align=\"center\">\n\
          <input type=\"text\" name=\"e6\" id=\"e6\" style=\"width: 100%;\" maxlength=\"2\">\n\
          </div>\n\
      </label></td>\n\
      <td><label>\n\
        <div align=\"center\">\n\
          <input type=\"text\" name=\"f6\" id=\"f6\" style=\"width: 100%;\" maxlength=\"2\">\n\
          </div>\n\
      </label></td>\n\
      <td><label>\n\
        <div align=\"center\">\n\
          <input type=\"text\" name=\"t6\" id=\"t6\" style=\"width: 100%;\" maxlength=\"3\">\n\
          </div>\n\
      </label></td>\n\
      </tr>\n\
    <tr>\n\
      <td><div align=\"center\">7</div></td>\n\
      <td><label>\n\
        <div align=\"center\">\n\
          <input type=\"text\" name=\"e7\" id=\"e7\" style=\"width: 100%;\" maxlength=\"2\">\n\
          </div>\n\
      </label></td>\n\
      <td><label>\n\
        <div align=\"center\">\n\
          <input type=\"text\" name=\"f7\" id=\"f7\" style=\"width: 100%;\" maxlength=\"2\">\n\
          </div>\n\
      </label></td>\n\
      <td><label>\n\
        <div align=\"center\">\n\
          <input type=\"text\" name=\"t7\" id=\"t7\" style=\"width: 100%;\" maxlength=\"3\">\n\
          </div>\n\
      </label></td>\n\
      </tr>\n\
    <tr>\n\
      <td><div align=\"center\">8</div></td>\n\
      <td><label>\n\
        <div align=\"center\">\n\
          <input type=\"text\" name=\"e8\" id=\"e8\" style=\"width: 100%;\" maxlength=\"2\">\n\
          </div>\n\
      </label></td>\n\
      <td><label>\n\
        <div align=\"center\">\n\
          <input type=\"text\" name=\"f8\" id=\"f8\" style=\"width: 100%;\" maxlength=\"2\">\n\
          </div>\n\
      </label></td>\n\
      <td><label>\n\
        <div align=\"center\">\n\
          <input type=\"text\" name=\"t8\" id=\"t8\" style=\"width: 100%;\" maxlength=\"3\">\n\
          </div>\n\
      </label></td>\n\
      </tr>\n\
    <tr>\n\
      <td><div align=\"center\">9</div></td>\n\
      <td><label>\n\
        <div align=\"center\">\n\
          <input type=\"text\" name=\"e9\" id=\"e9\" style=\"width: 100%;\" maxlength=\"2\">\n\
          </div>\n\
      </label></td>\n\
      <td><label>\n\
        <div align=\"center\">\n\
          <input type=\"text\" name=\"f9\" id=\"f9\" style=\"width: 100%;\" maxlength=\"2\">\n\
          </div>\n\
      </label></td>\n\
      <td><label>\n\
        <div align=\"center\">\n\
          <input type=\"text\" name=\"t9\" id=\"t9\" style=\"width: 100%;\" maxlength=\"3\">\n\
          </div>\n\
      </label></td>\n\
    </tr>\n\
  </table>\n\
<br>\n\
\n\
 <p align=\"center\"><input class=\"btn btn--orange btn--radius\" type=\"submit\" name=\"sn\" value=\"Submit preset data\"/></p>\n\
<script language=\"javascript\">\n";


char index_html2[]=
"document.getElementById('e0').value = 1; \n\
 document.getElementById('f0').value = 2; \n\
 document.getElementById('t0').value = 3; \n\
\n\
 document.getElementById('e1').value = 11; \n\
 document.getElementById('f1').value = 12; \n\
 document.getElementById('t1').value = 13; \n\
 \n\
 document.getElementById('e2').value = 21; \n\
 document.getElementById('f2').value = 22; \n\
 document.getElementById('t2').value = 23; \n\
 \n\
 document.getElementById('e3').value = 31; \n\
 document.getElementById('f3').value = 32; \n\
 document.getElementById('t3').value = 33; \n\
 \n\
 document.getElementById('e4').value = 41; \n\
 document.getElementById('f4').value = 42; \n\
 document.getElementById('t4').value = 43; \n\
 \n\
 document.getElementById('e5').value = 51; \n\
 document.getElementById('f5').value = 52; \n\
 document.getElementById('t5').value = 53; \n\
 \n\
 document.getElementById('e6').value = 61; \n\
 document.getElementById('f6').value = 62; \n\
 document.getElementById('t6').value = 63; \n\
 \n\
 document.getElementById('e7').value = 71; \n\
 document.getElementById('f7').value = 72; \n\
 document.getElementById('t7').value = 73; \n\
 \n\
 document.getElementById('e8').value = 81; \n\
 document.getElementById('f8').value = 82; \n\
 document.getElementById('t8').value = 83; \n\
 \n\
 document.getElementById('e9').value = 91; \n\
 document.getElementById('f9').value = 92; \n\
 document.getElementById('t9').value = 93; ";

char index_html3[]=
"</script>\n\
\n\
</form>\n";

char index_html4[]=
"</body>\n\
</html>\n";

