(function ($) {
  Drupal.behaviors.interact = {
    attach: function (context, settings) {
$("h2 a").css("textTransform", "capitalize");
$('h2 a').each(function() {
$(this).attr("href", "");
});

}
}
})(jQuery);