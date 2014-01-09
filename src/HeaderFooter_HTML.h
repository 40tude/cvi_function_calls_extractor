const char *Header_HTML="                                                                                                                      \                       
<!DOCTYPE HTML>                                                                                                                                \                       
<html lang=\"en\">                                                                                                                             \                       
  <head>                                                                                                                                       \                       
    <title>CVI Sample Cross Table</title>                                                                                                      \                       
    <meta charset=\"utf-8\">                                                                                                                   \                       
                                                                                                                                               \                       
    <style type=\"text/css\">                                                                                                                  \                       
    html, body, div, span, applet, object, iframe, h1, h2, h3, h4, h5, h6, p, blockquote, pre, a, abbr, acronym, address, big, cite, code, del,\                       
dfn, em, img, ins, kbd, q, s, samp, small, strike, strong, sub, sup, tt, var, b, u, i, center, dl, dt, dd, ol, ul, li, fieldset, form, label,  \                       
legend, table, caption, tbody, tfoot, thead, tr, th, td, article, aside, canvas, details, embed, figure, figcaption, footer, header, hgroup,   \                       
menu, nav, output, ruby, section, summary, time, mark, audio, video { margin: 0; padding: 0; border: 0; font-size: 100%; font: inherit;        \                       
vertical-align: baseline; } article, aside, details, figcaption, figure, footer, header, hgroup, menu, nav, section { display: block; }        \                       
body {line-height: 1;} ol, ul { list-style: none; } blockquote, q { quotes: none; } blockquote:before, blockquote:after, q:before,             \                       
q:after { content: ''; content: none; } table { border-collapse: collapse; border-spacing: 0; }                                                \                       
    </style>                                                                                                                                   \                       
                                                                                                                                               \                       
    <style type=\"text/css\">                                                                                                                  \                       
      body {                                                                                                                                   \                       
        font-family : Verdana;                                                                                                                 \                       
        font-size : 100%;                                                                                                                      \                       
        margin : 0 10%                                                                                                                         \                       
      }                                                                                                                                        \                       
      p{                                                                                                                                       \                       
        margin-top : 1em;                                                                                                                      \                       
      }                                                                                                                                        \                       
                                                                                                                                               \                       
      h1{                                                                                                                                      \                       
        font-size : 2em;                                                                                                                       \                       
        margin-top : 1em;                                                                                                                      \                       
      }                                                                                                                                        \                       
      a:link{                                                                                                                                  \                       
        text-decoration: none;                                                                                                                 \                       
        color:#e78f08;                                                                                                                         \                       
      }                                                                                                                                        \                       
      a:visited{ color:#e78f08;}                                                                                                               \                       
                                                                                                                                               \                       
      .author{                                                                                                                                 \                       
        margin-top : 0em;                                                                                                                      \                       
        font-size : 0.8em;                                                                                                                     \                       
      }                                                                                                                                        \                       
                                                                                                                                               \                       
      .Cadre {                                                                                                                                 \                       
        border : 3px solid #e78f08;                                                                                                            \                       
        border-spacing : 0;                                                                                                                    \                       
        margin-top : 1em;                                                                                                                      \                       
      }                                                                                                                                        \                       
                                                                                                                                               \                       
      .Cell_Small {                                                                                                                            \                       
        border : thin solid #f6a828;                                                                                                           \                       
        font-size : 0.9em;                                                                                                                     \                       
        //width : 30px;                                                                                                                        \                       
        //height : 30px;                                                                                                                       \                       
        text-align : left;                                                                                                                     \                       
        vertical-align : middle;                                                                                                               \                       
      }                                                                                                                                        \                       
    </style>                                                                                                                                   \                       
                                                                                                                                               \                       
  </head>                                                                                                                                      \                       
";

const char *Body1_HTML="                                                                                                                    \                          
  <body>                                                                                                                                    \                          
    <header>                                                                                                                                \                          
      <h1>CVI Sample Cross Table</h1>                                                                                                       \                          
      <p class=\"author\">by 40tude (see <a href=\"http://www.40tude.fr/blog/\" target=\"_blank\">www.40tude.fr/blog/</a> for more info)</p>\                          
    </header>                                                                                                                               \                          
                                                                                                                                            \                          
    <section>                                                                                                                               \                          
";

const char *Body2_HTML="                               \                                                                                                                                         
      <table class=\"Cadre\">                          \                                                                                                               
        <tr>                                           \                                                                                                               
          <td class=\"Cell_Small\">Function's name</td>\                                                                                                               
          <td class=\"Cell_Small\"># of calls</td>     \                                                                                                               
        </tr>                                          \                                                                                                               
          <!-- Insert below -->                        \                                                                                                               
";

const char *Footer_HTML ="     \   
          <!-- Insert above -->\   
      </table>                 \   
    </section>                 \   
  </body>                      \   
</html>                        \   
";                   
