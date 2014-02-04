(function ($) {
  //Drupal behavior for our module.
  Drupal.behaviors.stripe = {
    attach: function(context, settings) {
     // Get value of publishable key.

      var key = $("#ebookstripe_publishable_key").val();

      Stripe.setPublishableKey(key);
      //Select submit-button on click because we have a back button.
      $("#stripesubmit").click(function(event) {
        // Disable the submit button.
        $('#stripesubmit').attr("disabled", "disabled");
        
        Stripe.createToken({
          number: $('.card-number').val(),
          cvc: $('.card-cvc').val(),
          exp_month: $('.card-expire-month').val(),
          exp_year: $('.card-expire-year').val()
        }, stripeResponseHandler);

        // Stop returns.
        return false;
      });
      
      function stripeResponseHandler(status, response) {
        if (response.error) {
          // Display errors.
          $(".payment-errors").text("Error: " + response.error.message);
          $("#stripesubmit").removeAttr("disabled");
        } else {
        var form$ = $("#ebookstripe-form");
        var token = response['id'];
        // Add token value and remove name on card values so they are not passed.
        $("input[name=stripetoken]").val(token);
        $(".card-number").removeAttr("name");
        $(".card-cvc").removeAttr("name");
        $(".card-expire-month").removeAttr("name");
        $(".card-expire-yaer").removeAttr("name");
        // and submit
       // form$.get(0).submit();
       $("#ebookstripe-create-form").submit();
       } 
     }
   }
  };

})(jQuery);