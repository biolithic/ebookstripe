(function ($) {
  Drupal.behaviors.interact = {
    attach: function (context, settings) {

var hashes = window.location.href.split('/');
var chaptertitle = hashes[5] + " " + hashes[6];
$(".title").html(chaptertitle).css("textTransform", "capitalize");
$("h2 a").css("display", "none");

$('h2 a').each(function() {
var stuff = "<a href='" + $(this).attr('href') + "/edit' title='Edit this section'>Edit section " + $(this).text() + "</a><br />";
$('.blockInner').append(stuff);
});

$("form#user-login-form div div.item-list ul li.first").css("display", "none");
$("form#user-login-form div div.item-list ul li.last").css("display", "none");
$("div#block-user-login.block.block-user div.blockInner a").css("display", "none");
$('#edit-actions').append("Demo credentials<br />This user will have ability to fully read chapters one and two.<br />testuser : password");
 }
  }
})(jQuery);