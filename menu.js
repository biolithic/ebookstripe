(function ($) {
  Drupal.behaviors.interact = {
    attach: function (context, settings) {

var getthepage = document.URL;
console.log(getthepage);
if (getthepage.indexOf("chapter") != -1)
{
if (getthepage === "http://andymartha.com/appsinactionbook/chapter1")
{
var n2 = 2;
jQuery('#content-wrapper').append("<div id='stuff2' style='background:transparent; position:fixed; bottom: 72px; right: 0px; width:7%; height:65px; font-size:3em; text-align:center; text-shadow: 0px 2px 2px rgba(0, 0, 0, 0.4);'><a href='chapter" + n2 + "'>" + n2 + "</a></div>");
}
else if (getthepage === "http://andymartha.com/appsinactionbook/chapter18")
{
var n2 = 17;
jQuery('#content-wrapper').append("<div id='stuff2' style='background:transparent; position:fixed; bottom: 72px; left: 0px; width:7%; height:65px; font-size:3em; text-align:center; text-shadow: 0px 2px 2px rgba(0, 0, 0, 0.4);'><a href='chapter" + n2 + "'>" + n2 + "</a></div>");
}
else
{
console.log("this is a chapter!");
var anumber = getthepage.slice(-2);
if (isNaN(anumber) == true)
{
console.log("this not a number");
var anumber = getthepage.slice(-1);  
}
console.log(anumber);
var n1 = anumber - 1;
var n2 = anumber - -1;
jQuery('#content-wrapper').append("<div id='stuff1' style='background:transparent; position:fixed; bottom: 72px; left: 0px; width:7%; height:65px; font-size:3em; text-align:center; text-shadow: 0px 2px 2px rgba(0, 0, 0, 0.7);'><a href='chapter" + n1 + "'>" + n1 + "</a></div>");
jQuery('#content-wrapper').append("<div id='stuff2' style='background:transparent; position:fixed; bottom: 72px; right: 0px; width:7%; height:65px; font-size:3em; text-align:center; text-shadow: 0px 2px 2px rgba(0, 0, 0, 0.4);'><a href='chapter" + n2 + "'>" + n2 + "</a></div>");

}
}  
}    
}
 })(jQuery);


