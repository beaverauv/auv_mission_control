

//height = height of box found by cnn
//width = ^but width
//p = height / width
//theta = atan((8p-1)/(8-p))
//important to note that that angle is left/right ambiguous
//check two top quadrants of box. See which has more orange
//if ^ == left, theta *= -1
