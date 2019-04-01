$(document).ready(function(){
$('.carousel').carousel();
});

var instance = M.Carousel.getInstance(elem);

instance.next();
instance.next(3); // Move next n times.