<?php

/**
 * @file
 * Module to accept billing information and process transactions with ebookstripe.
 */

 
/**
 * Implements hook_help().
 */
function ebookstripe_help($path, $arg) {
  switch ($path) {
    case 'admin/help#ebookstripe':
      return t('<p>Instantstripe help here.</p>');
    case 'admin/settings/ebookstripe':
      return t('<p>Instantstripe help here.</p>');
  }
}



/**
 * Implements hook_menu().
 */
function ebookstripe_menu() {
  $items = array();

  $items['admin/config/services/ebookstripe/reports'] = array(
    'title' => 'EbookStripe Sales Reports',
    'description' => 'Who has bought your book through StripeJS',
    'page callback' => 'ebookstripe_reports_view',
    'access arguments' => array('access content'),
    'weight' => 2,
    'type' => MENU_CALLBACK,
  );

  $items['admin/config/services/ebookstripe/readme'] = array(
    'title' => 'EbookStripe set up instructions',
    'description' => 'Read Me file',
    'page callback' => 'ebookstripe_readme_view',
    'access arguments' => array('access content'),
    'weight' => 3,
    'type' => MENU_CALLBACK,
  );

  $items['checkout'] = array(
    'title' => 'Checkout',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('ebookstripe_form'),
    'access callback' => TRUE,
  );
  $items['admin/config/services/ebookstripe'] = array(
    'title' => 'EbookStripe',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('ebookstripe_admin_settings'),
    'access arguments' => array('administer modules'),
    'description' => 'Configure Stripe keys provided by Stripe.com when you register there.',
  );
  return $items;
}

function ebookstripe_reports_view() {
  $query = db_select('ebookstripe_customers', 'n')->fields('n', array('thid', 'timestamp', 'contactname', 'contactemail', 'booktype'))->extend('PagerDefault')->orderBy('timestamp', 'DESC')->limit(10);

  // Fetch the result set.
  $result = $query->execute();
  $num_rows = $query->countQuery()->execute()->fetchField();
  if ($num_rows < 1) {
    $output = "You have sold no products";
  }
  else {
    // Loop through each item and add to the $rows array.
    foreach ($result as $row) {
      $rows[] = array(
        $row->thid,
        $row->contactname,
        $row->contactemail,
        format_interval(REQUEST_TIME - $row->timestamp) . ' ' . t('ago'),
        $row->booktype,
      );
    }

    // Headers for theme_table().
    $header = array(
      'ID',
      'Name',
      'Email',
      'Date',
      'Book Type',
    );

    // Format output.
    $output = "You have sold " . $num_rows . " products";
    $output .= theme('table', array('header' => $header, 'rows' => $rows)) . theme('pager');
  }
  return $output;
}

function ebookstripe_readme_view() {
  // Format output.
  $build = array(
    'render_array_paragraph' => array(
      '#type' => 'markup',
      '#markup' => '<h2>Congradulations on installing the EbookStripe module.</h2><p>This is an e-commerce module for processing payment using StripeJS for single digital items, if you do not feel like using an entire store like Ubercart or Commerce.',
    ),
    'why_render_arrays' => array(
      '#items' => array(
        'Drupal 7 module for Stripe Payments This is an e-commerce module for processing payment using StripeJS for single digital items, if you do not feel like using an entire store like Ubercart or Commerce.',
        '',
        'This is made to be simple to set up and sell, but not plug-and-play',
        'For larger volume, please use Drupal Commerce or Ubercart',
        'This does not work with other payment gateways such as Paypal or Authorize.net',
        'This requires Javascript to be enabled at all times to function',
        'Stripe and this module do not interact with Drupal in regards to credit card information',
        'Perceived functionality is selling a single digital item like an ebook on your Drupal site',
        '...without a huge ecommerce system or footprint',
        'Installing ',
        '- Install the module in drupals /sites/all/modules like all other modules',
        '- the module will install a product node type, product fields, a checkout page and a sales report page.',
        '',
        'Configuring',
        '',
        '- Set up your banking information and get approved at Stripe.com - this will take a day or week',
        '- Open the webpage admin/config/services/ebookstripe and enter your Stripe details and options and click save',
        '- Stripe will give you a public key pair to test with, so enter these in and DO NOT check the live? box',
        '- In your Drupal site, add content of type EbookStripe, fill in your ebook or product details, and click save.',
        '- When site users view a node of that type, they can immediately click the checkout button to purchase it on site with their credit card.',
        '- Use credit card number 4242424242424242 and cvc number 123 to test.',
        '- Visit https://manage.stripe.com/test/dashboard, sign in, go to your reports, and you should see your credit card transaction show up.',
        '- Repeat the above steps with your live account info from Stripe (and small amounts of money to test).',
        '- Open the webpage admin/config/services/ebookstripe and enter your LIVE Stripe details, check the live? box and click save',
        '- You are now open for business!',
        '- Drupal does not receive or save any transaction info besides the customer name, email, and what format of book they want!',
        '- This is a basic starter kit with which you can alter to your liking',
        '- Go to admin/config/services/ebookstripe/reports for a simple report of your successful sales in Drupal',
        '- Uninstalling this module removes all of your products and customer database info!',
        '',
        'To Do:',
        '',
        '- better reporting page. Keep in mind, this will not save anything about the credit cards to be PCI-compliant.',
        '- more configuration.',
      ),
      '#title' => 'How to use:',
      '#theme' => 'item_list',
    ),
  );

  return $build;
}

/**
 * Configuration Form for Admin to set the secret and publishable keys.
 *
 * @return array
 *   Array of form values to be saved.
 */
function ebookstripe_admin_settings() {
  $form = array(
    '#type' => 'fieldset',
    '#title' => t('My Module Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['ebookstripe_readme'] = array(
    '#type' => 'markup',
    '#title' => t('Read me instructions'),
    '#markup' => '<p>' . l(t('Read me instructions'), 'admin/config/services/ebookstripe/readme') . '</p>',
  );

  $form['ebookstripe_publishable_testkey'] = array(
    '#type' => 'textfield',
    '#title' => t('Stripe test public API Key'),
    '#default_value' => variable_get('ebookstripe_publishable_testkey', ""),
    '#description' => t('Stripe test public API Key'),
    '#required' => TRUE,
  );

  $form['ebookstripe_publishable_livekey'] = array(
    '#type' => 'textfield',
    '#title' => t('Stripe live public API Key'),
    '#default_value' => variable_get('ebookstripe_publishable_livekey', ""),
    '#description' => t('Stripe live public API Key'),
    '#required' => TRUE,
  );

  $form['ebookstripe_secret_testkey'] = array(
    '#type' => 'textfield',
    '#title' => t('Stripe test secret API Key'),
    '#default_value' => variable_get('ebookstripe_secret_testkey', ""),
    '#description' => t('Stripe test secret API Key'),
    '#required' => TRUE,
  );

  $form['ebookstripe_secret_livekey'] = array(
    '#type' => 'textfield',
    '#title' => t('Stripe live secret API Key'),
    '#default_value' => variable_get('ebookstripe_secret_livekey', ""),
    '#description' => t('Stripe live secret API Key'),
    '#required' => TRUE,
  );

  $form['ebookstripe_price'] = array(
    '#type' => 'textfield',
    '#title' => t('Price of download'),
    '#default_value' => variable_get('ebookstripe_price', ""),
    '#description' => t('Price of download'),
    '#required' => TRUE,
  );

  $form['ebookstripe_gonelive'] = array(
    '#type' => 'checkbox',
    '#title' => t('Live?'),
    '#default_value' => variable_get('ebookstripe_gonelive', ""),
    '#description' => t('Check this box to use your live key.  Uncheck to enter demo mode and use your test key'),
  );
  
  $form['ebookstripe_sendemail'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send emails to admin and customer for each successful transaction?'),
    '#default_value' => variable_get('ebookstripe_sendemail', ""),
    '#description' => t('Check this box to send emails.  Uncheck to not send emails.'),
  );

  $form['ebookstripe_reports'] = array(
    '#type' => 'markup',
    '#title' => t('Sales report'),
    '#markup' => '<p>' . l(t('See customer list'), 'admin/config/services/ebookstripe/reports') . '</p>',
  );

  $form['ebookstripe_fields_description'] = array(
    '#type' => 'markup',
    '#title' => t('Which fields do you want to collect?'),
    '#markup' => '<hr /><p>You have the choice to collect the following information from your customers below</p>',
  );

  $form['ebookstripe_firstname'] = array(
    '#type' => 'checkbox',
    '#title' => t('First Name?'),
    '#default_value' => variable_get('ebookstripe_firstname', ""),
    '#description' => t('Check this box to collect first names.  Uncheck to leave the first name box off of the checkout form.'),
  );
  
  $form['ebookstripe_lastname'] = array(
    '#type' => 'checkbox',
    '#title' => t('Last Name?'),
    '#default_value' => variable_get('ebookstripe_lastname', ""),
    '#description' => t('Check this box to collect last names.  Uncheck to leave the last name box off of the checkout form.'),
  );
  
  $form['ebookstripe_phone'] = array(
    '#type' => 'checkbox',
    '#title' => t('Phone Number?'),
    '#default_value' => variable_get('ebookstripe_phone', ""),
    '#description' => t('Check this box to collect phone numbers.  Uncheck to leave the phone number box off of the checkout form.'),
  );
  
  $form['ebookstripe_address'] = array(
    '#type' => 'checkbox',
    '#title' => t('Address 1?'),
    '#default_value' => variable_get('ebookstripe_address', ""),
    '#description' => t('Check this box to collect address 1.  Uncheck to leave the address 1 box off of the checkout form.'),
  );
  
  $form['ebookstripe_address2'] = array(
    '#type' => 'checkbox',
    '#title' => t('Address 2?'),
    '#default_value' => variable_get('ebookstripe_address2', ""),
    '#description' => t('Check this box to collect address 2.  Uncheck to leave the address 2 box off of the checkout form.'),
  );
  
  $form['ebookstripe_city'] = array(
    '#type' => 'checkbox',
    '#title' => t('City?'),
    '#default_value' => variable_get('ebookstripe_city', ""),
    '#description' => t('Check this box to collect city.  Uncheck to leave the city box off of the checkout form.'),
  );
  
  $form['ebookstripe_county'] = array(
    '#type' => 'checkbox',
    '#title' => t('County?'),
    '#default_value' => variable_get('ebookstripe_county', ""),
    '#description' => t('Check this box to collect county.  Uncheck to leave the county box off of the checkout form.'),
  );
  
  $form['ebookstripe_state'] = array(
    '#type' => 'checkbox',
    '#title' => t('State?'),
    '#default_value' => variable_get('ebookstripe_state', ""),
    '#description' => t('Check this box to collect state.  Uncheck to leave the state box off of the checkout form.'),
  );
  
  $form['ebookstripe_zip'] = array(
    '#type' => 'checkbox',
    '#title' => t('Zip?'),
    '#default_value' => variable_get('ebookstripe_zip', ""),
    '#description' => t('Check this box to collect zip codes.  Uncheck to leave the zip code box off of the checkout form.'),
  );
  
  return system_settings_form($form);
}

/**
 * First page of a multi-page form.
 * This page provides billing fields.
 *
 * @param array $form
 *   Define form elements.
 *
 * @param array $form_state
 *   State of current forms including passed values.
 *
 * @return array
 */
function ebookstripe_form($form, $form_state) {

  // Return payment section of form if page_num == 2.
  if (!empty($form_state['page_num']) && $form_state['page_num'] == 2) {
    return ebookstripe_form_payment($form, $form_state);
  }

  // Set page_num to 1 and build page 1.
  $form_state['page_num'] = 1;

  $form = array();

  // Define form as hierarchy so we can access both pages as trees.
  $form['#tree'] = TRUE;

  $form['billing'] = array(
    '#type' => 'fieldset',
    '#title' => t('Billing & Account Details'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  // Check for $form_state values returned on form rebuild and when submitting the back button.
  
  global $user;
  if($user->uid == 0)
  {
  $email = isset($form_state['values']['billing']['email']) ? $form_state['values']['billing']['email'] : '';
  }
  else
  {
  $email = $user->mail;  
  }
  $form['billing']['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#required' => TRUE,
    '#default_value' => $email,

    // Perform ajax validation.
    '#ajax' => array(
      'callback' => '_ebookstripe_form_email_ajax_validate',
      'wrapper' => 'email-error',
      'effect' => 'slide',
    ),
    '#suffix' => '<div id="email-error" style="color:#ff0000;"> </div>',
  );

  $form['billing']['password'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#required' => TRUE,
  );

  $form['billing']['conf-password'] = array(
    '#type' => 'password',
    '#title' => t('Confirm Password'),
    '#required' => TRUE,
  );

  $testvariable = variable_get('ebookstripe_firstname');
  if ($testvariable > 0) {
  $firstname = isset($form_state['values']['billing']['firstname']) ? $form_state['values']['billing']['firstname'] : '';
  $form['billing']['firstname'] = array(
    '#type' => 'textfield',
    '#title' => t('First Name'),
    '#required' => TRUE,
    '#default_value' => $firstname,
  );
  }
  
  $testvariable = variable_get('ebookstripe_lastname');
  if ($testvariable > 0) {
  $lastname = isset($form_state['values']['billing']['lastname']) ? $form_state['values']['billing']['lastname'] : '';
  $form['billing']['lastname'] = array(
    '#type' => 'textfield',
    '#title' => t('Last Name'),
    '#required' => TRUE,
    '#default_value' => $lastname,
  );
  }

  $testvariable = variable_get('ebookstripe_phone');
  if ($testvariable > 0) {
  $phone = isset($form_state['values']['billing']['phone']) ? $form_state['values']['billing']['phone'] : '';
  $form['billing']['phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Phone'),
    '#required' => TRUE,
    '#default_value' => $phone,
  );
  }

  $testvariable = variable_get('ebookstripe_address');
  if ($testvariable > 0) {
  $address = isset($form_state['values']['billing']['address']) ? $form_state['values']['billing']['address'] : '';
  $form['billing']['address'] = array(
    '#type' => 'textfield',
    '#title' => t('Address Line 1'),
    '#required' => TRUE,
    '#default_value' => $address,
  );
  }
  
  $testvariable = variable_get('ebookstripe_address2');
  if ($testvariable > 0) {
  $address2 = isset($form_state['values']['billing']['address2']) ? $form_state['values']['billing']['address2'] : '';
  $form['billing']['address2'] = array(
    '#type' => 'textfield',
    '#title' => t('Address Line 2'),
    '#default_value' => $address2,
  );
  }
  
  $testvariable = variable_get('ebookstripe_city');
  if ($testvariable > 0) {
  $city = isset($form_state['values']['billing']['city']) ? $form_state['values']['billing']['city'] : '';
  $form['billing']['city'] = array(
    '#type' => 'textfield',
    '#title' => t('City'),
    '#required' => TRUE,
    '#default_value' => $city,
  );
  }

  $testvariable = variable_get('ebookstripe_county');
  if ($testvariable > 0) {
  $county = isset($form_state['values']['billing']['county']) ? $form_state['values']['billing']['county'] : '';
  $form['billing']['county'] = array(
    '#type' => 'textfield',
    '#title' => t('County'),
    '#required' => TRUE,
    '#default_value' => $county,
  );
  }

  $testvariable = variable_get('ebookstripe_state');
  if ($testvariable > 0) {
  $state = isset($form_state['values']['billing']['state']) ? $form_state['values']['billing']['state'] : '';
  $form['billing']['state'] = array(
    '#type' => 'textfield',
    '#title' => t('State'),
    '#required' => TRUE,
    '#default_value' => $state,
  );
  }
  
  $testvariable = variable_get('ebookstripe_zip');
  if ($testvariable > 0) {
  $zip = isset($form_state['values']['billing']['zip']) ? $form_state['values']['billing']['zip'] : '';
  $form['billing']['zip'] = array(
    '#type' => 'textfield',
    '#title' => t('Zip'),
    '#required' => TRUE,
    '#size' => 10,
    '#maxlength' => 10,
    '#default_value' => $zip,
  );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Next',
    '#submit' => array('ebookstripe_form_tracker'),
    '#validate' => array('ebookstripe_form_billing_validate'),
  );
  return $form;
}

/**
 * Track what page the form is on and rebuild form.
 *
 * @param array $form
 *   Elements to make up the form.
 *
 * @param array $form_state
 *   Form element value state of current form.
 */
function ebookstripe_form_tracker($form, &$form_state) {
  $form_state['page_values'][1] = $form_state['values'];

  if (!empty($form_state['page_values'][2])) {
    $form_state['values'] = $form_state['page_values'][2];
  }

  // Define page number on form rebuild.
  $form_state['page_num'] = 2;
  $form_state['rebuild'] = TRUE;
}

/**
 * Set page_num to 1 and rebuild form when back button is clicked.
 *
 * @param array $form
 *   Form elements to build the form.
 *
 * @param array $form_state
 *   Form state elements to pass values.
 */
function ebookstripe_form_back($form, &$form_state) {
  $form_state['values'] = $form_state['page_values'][1];
  $form_state['page_num'] = 1;
  $form_state['rebuild'] = TRUE;
}

/**
 * Provide page two of form to collect payment information and process with ebookstripe.
 *
 * @param array $form
 *   Form elements to build the form.
 *
 * @param array $form_state
 *   Form state elements to pass values.
 *
 * @return array
 *   return form and form_state arrays.
 */
function ebookstripe_form_payment($form, $form_state) {
  // Include external ebookstripe.js and local ebookstripe JQuery.
  drupal_add_js('https://js.stripe.com/v1/', 'external');
  drupal_add_js(drupal_get_path('module', 'ebookstripe') . '/js/ebookstripe.js');

  //$form = array();
  $form['payment'] = array(
    '#type' => 'fieldset',
    '#title' => t('Payment Details'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['payment']['card'] = array(
    '#type' => 'textfield',
    '#title' => t('Card'),
    '#size' => 20,
    '#maxlength' => 16,
    '#description' => t('Enter your card number'),

    //  '#required' => TRUE,
    '#attributes' => array('class' => array('card-number')),
  );
  $form['payment']['cvc'] = array(
    '#type' => 'textfield',
    '#title' => t('CVC'),
    '#size' => 3,
    '#maxlength' => 3,
    '#description' => t('Enter the CVC number printed on the back of your card.'),

    //  '#required' => TRUE,
    '#attributes' => array('class' => array('card-cvc')),
  );

  $month_range = range('01', '12');
  $month_options = array_combine($month_range, $month_range);
  $form['payment']['exp_month'] = array(
    '#type' => 'select',
    '#title' => t('Month'),
    '#options' => $month_options,
    '#attributes' => array('class' => array('card-expire-month')),
  );

  $year_range = range(date('Y'), date('Y') + 5);
  $year_options = array_combine($year_range, $year_range);
  $form['payment']['exp_year'] = array(
    '#type' => 'select',
    '#title' => t('Year'),
    '#options' => $year_options,
    '#attributes' => array('class' => array('card-expire-year')),
    '#suffix' => '<span class="payment-errors" style="color:#ff0000;"> </span>',
  );

  $form['stripetoken'] = array(
    '#type' => 'hidden',
    '#attributes' => array('id' => array('stripetoken')),
  );

  $one = variable_get('ebookstripe_publishable_testkey');
  $two = variable_get('ebookstripe_publishable_livekey');
  $three = variable_get('ebookstripe_gonelive');
  if ($three > 0) {
    $publishable_api_key = $two;
  }
  else {
    $publishable_api_key = $one;
  }

  $form['ebookstripe_publishable_key'] = array(
    '#type' => 'hidden',
    '#value' => $publishable_api_key,
    '#attributes' => array('id' => array('ebookstripe_publishable_key')),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#submit' => array('ebookstripe_form_callback'),
    '#attributes' => array('id' => array('submit-button')),
  );

  $form['back'] = array(
    '#type' => 'submit',
    '#value' => t('<< Back'),
    '#submit' => array('ebookstripe_form_back'),
    '#limit_validation_errors' => array(),
  );

  return $form;
}

/**
 * Ajax validation on email and password.
 *
 * @param array $form
 *   Elements to build the form.
 *
 * @param array $form_state
 *   Values passed during form submission.
 *
 * @return string
 */
function _ebookstripe_form_email_ajax_validate($form, &$form_state) {
  // Validate as a true email format.
  if (filter_var($form_state['values']['billing']['email'], FILTER_VALIDATE_EMAIL) == FALSE) {
    $noerror = '<div id="email-error" style="color:#ff0000;">The email is not valid.</div>';
    return $noerror;
  }
  else {
    $error = '<div id="email-error"> </div>';
    return $error;
  }
}

function ebookstripe_check_customer($email) {
  $query = 'SELECT name FROM {users} where mail = :mail';
  $result = db_query($query, array(':mail' => $email))->fetchField();
  return $result;
}

/**
 * Validate billing section of form (page 1).
 *
 * @param array $form
 *   Elements to build the form.
 *
 * @param array $form_state
 *   Values passes during form submission.
 */
function ebookstripe_form_billing_validate($form, $form_state) {
  // Validate as a true email format.
  if (filter_var($form_state['values']['billing']['email'], FILTER_VALIDATE_EMAIL) == FALSE) {
    form_set_error('billing][email', t('This email is not valid.'));
  }

  if ($form_state['values']['billing']['password'] != $form_state['values']['billing']['conf-password']) {
    form_set_error('billing][conf-password', t('The passwords did not match.'));
  }
}

/**
 * Process ebookstripe Charge and save user data.
 *
 * @param array $form
 * @param array $form_state
 */
function ebookstripe_form_callback($form, $form_state) {
  // Require ebookstripe Library.
  // NOTE: Transition this to using the Library 2.0 API
  require_once("lib/stripe-php/lib/Stripe.php");

  $one = variable_get('ebookstripe_secret_testkey');
  $two = variable_get('ebookstripe_secret_livekey');
  $three = variable_get('ebookstripe_gonelive');
  if ($three > 0) {
    $ebookstripe_secret_key = $two;
  }
  else {
    $ebookstripe_secret_key = $one;
  }

  empty($ebookstripe_secret_key) ? NULL : Stripe::setApiKey($ebookstripe_secret_key);
  // Assign billing data for easier referencing.
  $billing_values = $form_state['page_values']['1'];

  $customer_id = ebookstripe_check_customer($billing_values['billing']['email']);
  $price = variable_get('ebookstripe_price') * 100;
  try {
    if (empty($customer_id)) {
      // Create a customer as referenced here https://stripe.com/docs/tutorials/charges.
      $customer = Stripe_Customer::create(array(
        "card" => $form_state['values']['stripetoken'],
        "description" => $billing_values['billing']['email'],
      ));

      // Charge the customer.
      $charge = Stripe_Charge::create(array(
        "amount" => $price,
        "currency" => "usd",
        "customer" => $customer->id,
      ));
    }
    else {
      Stripe_Charge::create(array(
        "amount" => $price,
        "currency" => "usd",
        "customer" => $customer_id,
      ));
    }
  }
  catch (Stripe_CardError $e) {
    //Card is declined.
    $jbody = $e->getJsonBody();
    $err = $jbody['error'];
    watchdog('strip_card_error', 'Card error Type: @type, Code: @code, Message: @message', array(
      '@type' => $err['type'],
      '@code' => $err['code'],
      '@message' => $err['message'],
    ));
    drupal_set_message($err['message']);
    return;
  }
  catch (Stripe_InvalidRequestError $e) {
    //Invalid request error.
    $jbody = $e->getJsonBody();
    $err = $jbody['error'];
    watchdog('strip_card_request_error', 'Invalid Request Param: @param, Message: @message', array(

      //      '@param' => $err['param'],
      '@message' => $err['message'],
    ));
    drupal_set_message($err['message']);
    return;
  }
  catch (Stripe_AuthenticationError $e) {
    //Invalid Key request error.
    $jbody = $e->getJsonBody();
    $err = $jbody['error'];
    watchdog('stripe_invalid_key', 'Key error Type: @type, Message: @message', array(
      '@type' => $err['type'],
      '@message' => $err['message'],
    ));
    drupal_set_message($err['message']);
    return;
  }
  catch (Stripe_Error $e) {
    // General Error.
    $jbody = $e->getJsonBody();
    $err = $jbody['error'];
    watchdog('stripe_general_error', 'Message: @message', array(
      '@message' => $err['message'],
    ));
    drupal_set_message(t('A error occured, contact the site administrator.'));
    return;
  }
  catch (Exception $e) {
    watchdog('error', 'Message: @message', array(
      '@message' => $err['message'],
    ));
    drupal_set_message(t('A error occured, contact the site administrator.'));
    return;
  }

  // If no errors and the drupal account was not found create a new user.
  if (empty($customer_id)) {
    // Create User
    $array = explode("@", $billing_values['billing']['email']);
    $customername = $array[0];
    $create = array();
    $create['name'] = $customer->id;
    $create['pass'] = $billing_values['billing']['password'];
    $create['mail'] = $billing_values['billing']['email'];
    $create['status'] = 1; // Set status to active.
    $create['access'] = 0;
    $create['login'] = 0;
    $create['timezone'] = variable_get('date_default_timezone', '');
    $create['data'] = FALSE;
    user_save(NULL, $create);
  }

  
  $nid = db_insert('ebookstripe_customers')->fields(array(
    'timestamp' => REQUEST_TIME,
    'contactname' => $customer_id,
    'contactemail' => $billing_values['billing']['email'],
    'booktype' => 'pdf',
  ))->execute();

  drupal_set_message(t('Your order has been processed.'));
  
  $testvariable = variable_get('ebookstripe_sendemail');
  if ($testvariable > 0) {
  $module = 'ebookstripe';
  $key = 'key';
  $to = $billing_values['billing']['email'];

  $language = language_default();
  $params = array();
  $from = NULL;
  $send = FALSE;
  $message = drupal_mail($module, $key, $to, $language, $params, $from, $send);
  $sitename = variable_get('site_name', '');
  $siteadmin = variable_get('site_mail', '');
  $subject = 'Receipt for order from ' . $sitename;
  $message['headers']['CC'] = $siteadmin;
  $message['subject'] = $subject;
  $message['body'] = array();
  $message['body'][] = $to . ", login to " . $_SERVER['HTTP_HOST'] . $_SERVER['REQUEST_URI'] . " to see your order for $" . variable_get('ebookstripe_price') . " on " . date('l jS \of F Y h:i:s A');

  // Retrieve the responsible implementation for this message.
  $system = drupal_mail_system($module, $key);

  // Format the message body.
  $message = $system->format($message);

  // Send e-mail.
  $message['result'] = $system->mail($message);

  if ($message['result'] == TRUE) {
    drupal_set_message(t('Your receipt has been sent. Thank you!'));
  }
  else {
    drupal_set_message(t('There was a problem sending your receipt and it was not sent.'), 'error');
  }
  }
drupal_goto();
}



function ebookstripe_node_view($node, $view_mode) {
  if ($view_mode != 'rss') {
    if ($node->type == 'ebookstripe' && (arg(1) != $node->uid)) {
      $node->content['node'] = array(
        '#markup' => "<a href='checkout' id='checkoutbutton' name='checkoutbutton'>Checkout</a>",
        '#weight' => 100,
      );
    }
  }
}