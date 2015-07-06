PLOTVERBOSITY 0
PARSEVERBOSITY 0
GRAPHVERBOSITY 0
AGGREGATEVERBOSITY 0

################################################################################

NEWAGGREGATE
fragment           (preface name)  (start #)  (end #)  (step size)
affix              (prefix)  (postfix)
!aggregate

SLHA
slha_input          (column name)   (block name)     (block number)
correlateinput      (0/1)
!read

LHCO
lhco_input         (object type)  (column name)  (number in energy order)
input              (extra constant column)
condition          (formula)
!read

#############################################################################

AGGREGATE   (output file)

#############################################################################

NEWPLOT
xname         (x axis name)
logX          (yes/no)
xrange        (lower limit)     (upper limit)
yname         (x axis name)
logY          (x axis name)
yrange        (lower limit)     (upper limit)
zrange        (z lower limit)     (z upper limit)
legend        (x lower)  (x upper)  (y lower)  (y upper)
leftmargin    (% width)
rightmargin   (% width)
topmargin     (% width)
bottommargin  (% width)
!makecanvas

CURVE
file               (file name)
skip_lines         (# lines to skip)
input              (forumula for x input)
input              (forumula for y input)
line_width         (number)
point/line_style   (number)
point/line_color   (number)
draw_options       (ROOT plot code)
condition          (logical formula)
!read


CONTOUR
file               (file name)
skip_lines         (# lines to skip)
input              (forumula for x input)
input              (forumula for y input)
input              (forumula for z input)
xbins              (number x bins)
ybins              (number y bins)
draw_options       (ROOT plot code)
condition          (logical formula)
legend_entry       (string)
contour_levels     (list of contour levels)
condition          (logical formula)
!read


HISTOGRAM
file               (file name)
skip_lines         (# lines to skip)
input              (forumula for x input)
input              (forumula for y input)
line_width         (number)
line_style         (number)
line_color         (number)
point_size         (number)
point_color        (number)
point_style        (number)
fill_color         (number)
fill_style         (number)
draw_options       (ROOT plot code)
xbins              (number x bins)
legend_entry       (string)
condition          (logical formula)
!read


LHCOHIST
file               (file name)
lhco_input         (object type)  (column name)  (number in energy order)
lhco_input         (additional inputs for cuts)
input              (other inputs for cuts)
line_width         (number)
point_size         (number)
line_style         (number)
line_color         (number)
point_color        (number)
point_style        (number)
fill_color         (number)
fill_style         (number)
draw_options       (ROOT plot code)
xbins              (number x bins)
legend_entry       (string)
condition          (logical formula)
!read

################################################################################

PLOT   (output file)

################################################################################
