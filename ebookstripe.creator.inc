<?php

/**
 * This is the form the admin uses to auto-generate the necessary nodes and images for his/her e-book.
 */
function ebookstripe_creator_form($form, $form_state) {
$form = array();
$form['creator'] = array(
    '#type' => 'fieldset',
    '#title' => t('E-Book Auto-Generator for you'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['creator']['readme1'] = array(
    '#type' => 'markup',
    '#title' => t('Read me instructions'),
    '#markup' => '<p>Pushing the create button at the bottom of this page will generate the necessary pages and asset holders for your pay-per-chapter e-book.  Visitors can come and view an introduction of your chapter along with the images and a BUY NOW button.  If they push the button, enter in their credit card details, and submit, they can then view the entire chapter.</p>',
  );

$form['creator']['booktitle'] = array(
    '#type' => 'textfield',
    '#title' => t('What is the title of your book?'),
    '#required' => TRUE,
    '#ajax' => array(
      'callback' => '_ebookstripe_form_notnumeric_ajax_validate',
      'wrapper' => 'booktitle-error',
      'effect' => 'slide',
    ),
    '#suffix' => '<div id="booktitle-error" style="color:#ff0000;"> </div>',
  );
  
$form['creator']['chapters'] = array(
    '#type' => 'textfield',
    '#title' => t('How many chapters are in your book?'),
    '#required' => TRUE,
    '#ajax' => array(
      'callback' => '_ebookstripe_form_numeric1_ajax_validate',
      'wrapper' => 'chapters-error',
      'effect' => 'slide',
    ),
    '#suffix' => '<div id="chapters-error" style="color:#ff0000;"> </div>',
  );

$form['creator']['images'] = array(
    '#type' => 'textfield',
    '#title' => t('How many images or illustrations or screenshots do you roughly plan on entering for each chapter?'),
    '#required' => TRUE,
    '#default_value' => 0,
    '#ajax' => array(
      'callback' => '_ebookstripe_form_numeric2_ajax_validate',
      'wrapper' => 'images-error',
      'effect' => 'slide',
    ),
    '#suffix' => '<div id="images-error" style="color:#ff0000;"> </div>',
  );    
 
$form['creator']['ebookstripe_introduction'] = array(
    '#type' => 'checkbox',
    '#title' => t('Introduction?'),
    // '#default_value' => variable_get('ebookstripe_introduction', ""),
    '#description' => t('Check this box if your book should contain an introduction unrelated to the chapters.  Uncheck if you do not have this for your book.'),
  );

$form['creator']['ebookstripe_forward'] = array(
    '#type' => 'checkbox',
    '#title' => t('Forward?'),
    // '#default_value' => variable_get('ebookstripe_forward', ""),
    '#description' => t('Check this box if your book should contain a forward by another person.  Uncheck if you do not have this for your book.'),
  );

$form['creator']['ebookstripe_resources'] = array(
    '#type' => 'checkbox',
    '#title' => t('Resources?'),
    // '#default_value' => variable_get('ebookstripe_resources', ""),
    '#description' => t('Check this box if your book should contain an appendix or resources section at the end.  Uncheck if you do not have this for your book.'),
  );

$form['creator']['ebookstripe_whyshouldyouread'] = array(
    '#type' => 'checkbox',
    '#title' => t('Why Should You Read?'),
    // '#default_value' => variable_get('ebookstripe_whyshouldyouread', ""),
    '#description' => t('Check this box if your book should contain a small section about why the user should read it.  Uncheck if you do not have this for your book.'),
  );

$form['creator']['ebookstripe_copyright'] = array(
    '#type' => 'checkbox',
    '#title' => t('Copyright?'),
    // '#default_value' => variable_get('ebookstripe_copyright', ""),
    '#description' => t('Check this box if your book should contain a separate copyright section.  Uncheck if you do not have this for your book.'),
  );

$form['creator']['ebookstripe_abouttheauthor'] = array(
    '#type' => 'checkbox',
    '#title' => t('About the Author?'),
    // '#default_value' => variable_get('ebookstripe_abouttheauthor', ""),
    '#description' => t('Check this box if your book should contain an about the author section.  Uncheck if you do not have this for your book.'),
  );

$form['creator']['ebookstripe_conclusion'] = array(
    '#type' => 'checkbox',
    '#title' => t('Conclusion?'),
    // '#default_value' => variable_get('ebookstripe_conclusion', ""),
    '#description' => t('Check this box if your book should contain an small conclusion section.  Uncheck if you do not have this for your book.'),
  );

// $form['creator']['ebookstripe_tableofcontents'] = array(
//     '#type' => 'checkbox',
//     '#title' => t('Table of Contents?'),
//     '#default_value' => variable_get('ebookstripe_tableofcontents', ""),
//     '#description' => t('Check this box if your book should contain a simple one page table of contents.  Uncheck if you do not have this for your book.'),
//   );  
  
$form['creator']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Create',
    '#submit' => array('ebookstripe_form_creator'),
  );
  return $form;
} 



/**
 * This is the callback function that processes the e-book feature requests into the actual fields, terms and nodes.
 */
function ebookstripe_form_creator($form, $form_state) {
field_purge_batch(10);
drupal_flush_all_caches();

$chapters = $form_state['values']['chapters'];
$chapters = check_plain(filter_xss($chapters));
$images = $form_state['values']['images'];
$images = check_plain(filter_xss($images));
$booktitle = $form_state['values']['booktitle'];
$booktitle = str_replace(" ", "_", $booktitle);
$booktitle = check_plain(filter_xss($booktitle));
$ebookstripe_introduction = $form_state['values']['ebookstripe_introduction']; // 0 = not checked, 1 = checked
$ebookstripe_forward = $form_state['values']['ebookstripe_forward']; // 0 = not checked, 1 = checked
$ebookstripe_resources = $form_state['values']['ebookstripe_resources']; // 0 = not checked, 1 = checked
$ebookstripe_whyshouldyouread = $form_state['values']['ebookstripe_whyshouldyouread']; // 0 = not checked, 1 = checked
$ebookstripe_copyright = $form_state['values']['ebookstripe_copyright']; // 0 = not checked, 1 = checked
$ebookstripe_abouttheauthor = $form_state['values']['ebookstripe_abouttheauthor']; // 0 = not checked, 1 = checked
$ebookstripe_conclusion = $form_state['values']['ebookstripe_conclusion']; // 0 = not checked, 1 = checked
// $ebookstripe_tableofcontents = $form_state['values']['ebookstripe_tableofcontents']; // 0 = not checked, 1 = checked
$avariable = variable_get('ebookstripe_number_chapters');
$addition = $booktitle . "-" . $chapters;
$addition = $avariable . " " . $addition;
$avariable = variable_set('ebookstripe_number_chapters', $addition);

// CREATE CHAPTER FIELD
if (!field_info_field('field_ebookstripe_chapter')) {
$field = array(
  'field_name' => 'field_ebookstripe_chapter',
  'type' => 'taxonomy_term_reference',
  'label' => t('Chapter'),
  'settings' => array(
                'allowed_values' => array(
                    0 => array(
                        'vocabulary' => 'tags',
                        'parent' => '0',
                    ),
                ),
            ),
);
field_create_field($field);
}

// CREATE PIC FIELD
if (!field_info_field('field_ebookstripe_pic')) {
$field = array(
  'field_name' => 'field_ebookstripe_pic',
  'type' => 'image',
  'label' => t('Illustration'),
  );
$field = field_create_field($field);
}



$stuff = array('#type' => 'General Ebookstripe-book page');
$intersection = array_intersect($stuff, node_type_get_names());
if (!$intersection)
{
drupal_set_message("General Ebookstripe-book page not created yet");  
// Define the node type.
  $general = array(
    'type' => 'ebookstripe_general',
    'name' => t('General Ebookstripe-book page'),
    'base' => 'node_content',
    'description' => t('A general e-book page'),
    'body_label' => t('Content'),
  );
  // Set defaults.
  $content_type = node_type_set_defaults($general);
  // Add the body field (not part of default).
  node_add_body_field($content_type);
  node_type_save($content_type);

    // Add the field to the content type as a HTML select box.
$instance = array(
  'field_name' => 'field_ebookstripe_chapter',
  'entity_type' => 'node',
  'bundle' => 'ebookstripe_general',
  'label' => t('Chapter'),
  'description' => t('Which chapter does this content belong to?'),
  'required' => true,
  'display' => array(
    'teaser' => array(                           // Teaser display mode
      'label' => 'hidden',                       // Position of the label, ie. 'inline', 'above' or 'hidden'
    ),
    'default' => array(
      'label' => 'hidden',
    ),
    'teaser' => array(                           // Teaser display mode
      'type' => 'hidden',                       // Position of the label, ie. 'inline', 'above' or 'hidden'
    ),
    'default' => array(
      'type' => 'hidden',
    ),
  ),
  'widget' => array(
    'type' => 'options_select',
    'weight' => 3
    )
  );
field_create_instance($instance);
}


  



$stuff = array('#type' => 'beginning');
$intersection = array_intersect($stuff, node_type_get_names());
if (!$intersection)
{
drupal_set_message("beginning not created yet");
  // Define the node type.
  $beginning = array(
    'type' => 'ebookstripe_beginning',
    'name' => t('beginning'),
    'base' => 'node_content',
    'description' => t('The opening teaser of your chapter'),
    'body_label' => t('Content'),
  );
  // Set defaults.
  $content_type = node_type_set_defaults($beginning);
  // Add the body field (not part of default).
  node_add_body_field($content_type);
  node_type_save($content_type);
  
  // Add the field to the content type as a HTML select box.
$instance = array(
  'field_name' => 'field_ebookstripe_chapter',
  'entity_type' => 'node',
  'bundle' => 'ebookstripe_beginning',
  'label' => t('Chapter'),
  'description' => t('Which chapter does this content belong to?'),
  'required' => true,
  'display' => array(
    'teaser' => array(                           // Teaser display mode
      'label' => 'hidden',                       // Position of the label, ie. 'inline', 'above' or 'hidden'
    ),
    'default' => array(
      'label' => 'hidden',
    ),
    'teaser' => array(                           // Teaser display mode
      'type' => 'hidden',                       // Position of the label, ie. 'inline', 'above' or 'hidden'
    ),
    'default' => array(
      'type' => 'hidden',
    ),
  ),
  'widget' => array(
    'type' => 'options_select',
    'weight' => 3
    )
  );
field_create_instance($instance);
}


$stuff = array('#type' => 'EbookstripeButton');
$intersection = array_intersect($stuff, node_type_get_names());
if (!$intersection)
{
drupal_set_message("EbookstripeButton not created yet");  
  // Define the node type.
  $ib = array(
    'type' => 'ebookstripe_button',
    'name' => t('EbookstripeButton'),
    'base' => 'node_content',
    'description' => t('The Buy Now button'),
    'body_label' => t('Content'),
  );
  // Set defaults.
  $content_type = node_type_set_defaults($ib);
  // Add the body field (not part of default).
  node_add_body_field($content_type);
  node_type_save($content_type);
 
// Add the field to the content type as a HTML select box.
// Attach the field to our entity
$instance = array(
  'field_name' => 'field_ebookstripe_chapter',
  'entity_type' => 'node',
  'bundle' => 'ebookstripe_button',
  'label' => t('Chapter'),
  'description' => t('Which chapter does this content belong to?'),
  'required' => true,
  'display' => array(
    'teaser' => array(                           // Teaser display mode
      'label' => 'hidden',                       // Position of the label, ie. 'inline', 'above' or 'hidden'
    ),
    'default' => array(
      'label' => 'hidden',
    ),
    'teaser' => array(                           // Teaser display mode
      'type' => 'hidden',                       // Position of the label, ie. 'inline', 'above' or 'hidden'
    ),
    'default' => array(
      'type' => 'hidden',
    ),
  ),
  'widget' => array(
    'type' => 'options_select',
    'weight' => 3
    )
  );
field_create_instance($instance);
}

  
  if($images > 0)
  {
    $stuff = array('#type' => 'illustration');
    $intersection = array_intersect($stuff, node_type_get_names());
    if (!$intersection)
    {
    drupal_set_message("illustration not created yet");  
    $illustration = array(
    'type' => 'ebookstripe_illustration',
    'name' => t('illustration'),
    'base' => 'node_content',
    'description' => t('An illustration or screenshot for your chapter'),
    'body_label' => t('Content'),
  );
  // Set defaults.
  $content_type = node_type_set_defaults($illustration);
  // Add the body field (not part of default).
  node_add_body_field($content_type);
  node_type_save($content_type);
  
  // Add the field to the content type as a HTML select box.
// Add the field to the content type as a HTML select box.
$instance = array(
  'field_name' => 'field_ebookstripe_chapter',
  'entity_type' => 'node',
  'bundle' => 'ebookstripe_illustration',
  'label' => t('Chapter'),
  'description' => t('Which chapter does this content belong to?'),
  'required' => true,
  'display' => array(
    'teaser' => array(                           // Teaser display mode
      'label' => 'hidden',                       // Position of the label, ie. 'inline', 'above' or 'hidden'
    ),
    'default' => array(
      'label' => 'hidden',
    ),
    'teaser' => array(                           // Teaser display mode
      'type' => 'hidden',                       // Position of the label, ie. 'inline', 'above' or 'hidden'
    ),
    'default' => array(
      'type' => 'hidden',
    ),
  ),
  'widget' => array(
    'type' => 'options_select',
    'weight' => 3
    )
  );
field_create_instance($instance);

$instance = array(
    'field_name' => 'field_ebookstripe_pic',
    'entity_type' => 'node',
    'bundle' => 'ebookstripe_illustration',
    'label' => t('Illustration'),
    'description' => t('An image to prove your point about this chapter'),
    'settings' => array(
      'text_processing' => 0,
    ),
    'widget' => array('type' => 'image_image'),
    'weight' => 11,
    'display' => array(
      'teaser' => array(
        'label' => 'hidden',
        'type' => 'image',
        'settings' => array(
          'image_style' => 'thumbnail',
          'image_link' => '',
        ),
        'weight' => 0,
      ),
      'default' => array(
        'label' => 'hidden',
        'type' => 'image',
        'settings' => array(
          'image_link' => '',
        ),
      ),
    ),
  );
field_create_instance($instance);
}
}




  $stuff = array('#type' => 'cover');
    $intersection = array_intersect($stuff, node_type_get_names());
    if (!$intersection)
    {
    drupal_set_message("cover not created yet");  
    $cover = array(
    'type' => 'ebookstripe_cover',
    'name' => t('cover'),
    'base' => 'node_content',
    'description' => t('A cover for your book'),
    'body_label' => t('Content'),
  );
  // Set defaults.
  $content_type = node_type_set_defaults($cover);
  // Add the body field (not part of default).
  node_add_body_field($content_type);
  node_type_save($content_type);
  
  // Add the field to the content type as a HTML select box.
// Add the field to the content type as a HTML select box.
$instance = array(
  'field_name' => 'field_ebookstripe_chapter',
  'entity_type' => 'node',
  'bundle' => 'ebookstripe_cover',
  'label' => t('Chapter'),
  'description' => t('Which chapter does this content belong to?'),
  'required' => true,
  'display' => array(
    'teaser' => array(                           // Teaser display mode
      'label' => 'hidden',                       // Position of the label, ie. 'inline', 'above' or 'hidden'
    ),
    'default' => array(
      'label' => 'hidden',
    ),
    'teaser' => array(                           // Teaser display mode
      'type' => 'hidden',                       // Position of the label, ie. 'inline', 'above' or 'hidden'
    ),
    'default' => array(
      'type' => 'hidden',
    ),
  ),
  'widget' => array(
    'type' => 'options_select',
    'weight' => 3
    )
  );
field_create_instance($instance);

$instance = array(
    'field_name' => 'field_ebookstripe_pic',
    'entity_type' => 'node',
    'bundle' => 'ebookstripe_cover',
    'label' => t('Cover'),
    'description' => t('A cover for this book'),
    'settings' => array(
      'text_processing' => 0,
    ),
    'widget' => array('type' => 'image_image'),
    'weight' => 11,
    'display' => array(
      'teaser' => array(
        'label' => 'hidden',
        'type' => 'image',
        'settings' => array(
          'image_style' => 'thumbnail',
          'image_link' => '',
        ),
        'weight' => 0,
      ),
      'default' => array(
        'label' => 'hidden',
        'type' => 'image',
        'settings' => array(
          'image_link' => '',
        ),
      ),
    ),
  );
field_create_instance($instance);
}



  
for ($x=0; $x<=$chapters; $x++)
{
$chaptername = "chapter" . $x;

// MAKE ROLES
$role = new stdClass();
$role->name = $booktitle . "_" . $chaptername;
$auth_role = user_role_load(DRUPAL_AUTHENTICATED_RID);
user_role_save($role);

// MAKE TAXONOMY
$term = new stdClass();
$term->name = $booktitle . "_" . $chaptername;
$term->vid = 1; 
taxonomy_term_save($term);
}


  $node = new stdClass(); // We create a new node object
  $node->type = "ebookstripe_cover"; // Or any other content type you want
  $node->title = $booktitle;
  $node->language = LANGUAGE_NONE; // Or any language code if Locale module is enabled. More on this below *
  $node->uid = 1; // Or any id you wish
  $node->status = 1;
  $node->promote = 0;
  $node->comment = 1;
  node_object_prepare($node); // Set some default values.
  
  $term_name = $booktitle . "_" . "chapter0";
  $foo = taxonomy_get_term_by_name($term_name);
  foreach($foo as $term) {
  if($term->vid == 1) {
  $node->field_ebookstripe_chapter[$node->language][0]['tid'] = $term->tid;
    }
  }
  $node->body[$node->language][0]['value'] = "<a href='" . $booktitle . "/table_of_contents' title='open this book'>Read...</a>";
  $node->body[$node->language][0]['summary'] = "<a href='" . $booktitle . "/table_of_contents' title='open this book'>Read...</a>";
  $node->body[$node->language][0]['format'] = 'filtered_html'; 
  $node = node_submit($node); // Prepare node for a submit
  node_save($node); // After this call we'll get a nid  


$pagesarray = array();
if ($ebookstripe_introduction > 0)
{
  array_push($pagesarray, "Introduction");
  $node = new stdClass(); // We create a new node object
  $node->type = "ebookstripe_general"; // Or any other content type you want
  $node->title = "Introduction";
  $node->language = LANGUAGE_NONE; // Or any language code if Locale module is enabled. More on this below *
  $node->uid = 1; // Or any id you wish
  $nodepath = $booktitle . "/introduction";
  $node->path = array('alias' => $nodepath); // Setting a node path
  $node->status = 1;
  $node->promote = 0;
  $node->comment = 1;
  $term_name = $booktitle . "_" . "chapter0";
    $foo = taxonomy_get_term_by_name($term_name);
    foreach($foo as $term) {
        if($term->vid == 1) {
        $node->field_ebookstripe_chapter[$node->language][0]['tid'] = $term->tid;  
        }
    }
  node_object_prepare($node); // Set some default values.
  // Let's add standard body field
  $node->body[$node->language][0]['value'] = 'This is the Introduction to ' . $booktitle . '<br>Terry Richardson biodiesel chambray gentrify try-hard ethical. Meh ennui Brooklyn PBR&B Vice. Fap 90s mixtape, meh Pitchfork sartorial American Apparel cray tofu small batch tote bag ennui. Intelligentsia Echo Park fingerstache skateboard 3 wolf moon Neutra sartorial Truffaut pop-up. Swag synth lomo High Life messenger bag. Keffiyeh McSweeneys butcher jean shorts. Plaid YOLO twee biodiesel wayfarers. Roof party small batch Thundercats mustache gluten-free. Cred selvage whatever, drinking vinegar Portland vegan Odd Future butcher Austin distillery photo booth. Pinterest cred ethical, pork belly art party hella aesthetic mumblecore dreamcatcher forage brunch Neutra. Banjo chillwave shabby chic fap distillery, gentrify skateboard semiotics Marfa cliche American Apparel next level Truffaut Williamsburg';
  $node->body[$node->language][0]['summary'] = ' ';
  $node->body[$node->language][0]['format'] = 'full_html';
  $node = node_submit($node); // Prepare node for a submit
  node_save($node); // After this call we'll get a nid  
}


if ($ebookstripe_forward > 0)
{
  array_push($pagesarray, "Forward");
  $node = new stdClass(); // We create a new node object
  $node->type = "ebookstripe_general"; // Or any other content type you want
  $node->title = "Forward";
  $node->language = LANGUAGE_NONE; // Or any language code if Locale module is enabled. More on this below *
  $node->uid = 1; // Or any id you wish
  $nodepath = $booktitle . "/forward";
  $node->path = array('alias' => $nodepath); // Setting a node path
  $node->status = 1;
  $node->promote = 0;
  $node->comment = 1;
  $term_name = $booktitle . "_" . "chapter0";
    $foo = taxonomy_get_term_by_name($term_name);
    foreach($foo as $term) {
        if($term->vid == 1) {
        $node->field_ebookstripe_chapter[$node->language][0]['tid'] = $term->tid;  
        }
    }
  node_object_prepare($node); // Set some default values.
  // Let's add standard body field
  $node->body[$node->language][0]['value'] = 'This is the Forward to ' . $booktitle . '<br>Terry Richardson biodiesel chambray gentrify try-hard ethical. Meh ennui Brooklyn PBR&B Vice. Fap 90s mixtape, meh Pitchfork sartorial American Apparel cray tofu small batch tote bag ennui. Intelligentsia Echo Park fingerstache skateboard 3 wolf moon Neutra sartorial Truffaut pop-up. Swag synth lomo High Life messenger bag. Keffiyeh McSweeneys butcher jean shorts. Plaid YOLO twee biodiesel wayfarers. Roof party small batch Thundercats mustache gluten-free. Cred selvage whatever, drinking vinegar Portland vegan Odd Future butcher Austin distillery photo booth. Pinterest cred ethical, pork belly art party hella aesthetic mumblecore dreamcatcher forage brunch Neutra. Banjo chillwave shabby chic fap distillery, gentrify skateboard semiotics Marfa cliche American Apparel next level Truffaut Williamsburg';
  $node->body[$node->language][0]['summary'] = '  ';
  $node->body[$node->language][0]['format'] = 'full_html'; // If field has a format, you need to define it. Here we define a default filtered_html format for a body field
  $node = node_submit($node); // Prepare node for a submit
  node_save($node); // After this call we'll get a nid  
}

if ($ebookstripe_resources > 0)
{
  array_push($pagesarray, "Resources");
  $node = new stdClass(); // We create a new node object
  $node->type = "ebookstripe_general"; // Or any other content type you want
  $node->title = "Resources";
  $node->language = LANGUAGE_NONE; // Or any language code if Locale module is enabled. More on this below *
  $node->uid = 1; // Or any id you wish
  $nodepath = $booktitle . "/resources";
  $node->path = array('alias' => $nodepath); // Setting a node path
  $node->status = 1;
  $node->promote = 0;
  $node->comment = 1;
  $term_name = $booktitle . "_" . "chapter0";
    $foo = taxonomy_get_term_by_name($term_name);
    foreach($foo as $term) {
        if($term->vid == 1) {
        $node->field_ebookstripe_chapter[$node->language][0]['tid'] = $term->tid;  
        }
    }
  node_object_prepare($node); // Set some default values.
  // Let's add standard body field
  $node->body[$node->language][0]['value'] = 'This is the Resources to ' . $booktitle . '<br>Terry Richardson biodiesel chambray gentrify try-hard ethical. Meh ennui Brooklyn PBR&B Vice. Fap 90s mixtape, meh Pitchfork sartorial American Apparel cray tofu small batch tote bag ennui. Intelligentsia Echo Park fingerstache skateboard 3 wolf moon Neutra sartorial Truffaut pop-up. Swag synth lomo High Life messenger bag. Keffiyeh McSweeneys butcher jean shorts. Plaid YOLO twee biodiesel wayfarers. Roof party small batch Thundercats mustache gluten-free. Cred selvage whatever, drinking vinegar Portland vegan Odd Future butcher Austin distillery photo booth. Pinterest cred ethical, pork belly art party hella aesthetic mumblecore dreamcatcher forage brunch Neutra. Banjo chillwave shabby chic fap distillery, gentrify skateboard semiotics Marfa cliche American Apparel next level Truffaut Williamsburg';
  $node->body[$node->language][0]['summary'] = '  ';
  $node->body[$node->language][0]['format'] = 'full_html'; // If field has a format, you need to define it. Here we define a default filtered_html format for a body field
  $node = node_submit($node); // Prepare node for a submit
  node_save($node); // After this call we'll get a nid  
}

if ($ebookstripe_whyshouldyouread > 0)
{
  array_push($pagesarray, "Why Should You Read");
  $node = new stdClass(); // We create a new node object
  $node->type = "ebookstripe_general"; // Or any other content type you want
  $node->title = "Why Should You Read This Book?";
  $node->language = LANGUAGE_NONE; // Or any language code if Locale module is enabled. More on this below *
  $node->uid = 1; // Or any id you wish
  $nodepath = $booktitle . "/why_should_you_read";
  $node->path = array('alias' => $nodepath); // Setting a node path
  $node->status = 1;
  $node->promote = 0;
  $node->comment = 1;
  $term_name = $booktitle . "_" . "chapter0";
    $foo = taxonomy_get_term_by_name($term_name);
    foreach($foo as $term) {
        if($term->vid == 1) {
        $node->field_ebookstripe_chapter[$node->language][0]['tid'] = $term->tid;  
        }
    }
  node_object_prepare($node); // Set some default values.
  // Let's add standard body field
  $node->body[$node->language][0]['value'] = 'This is the Why Should You Read This Book to ' . $booktitle . '<br>Terry Richardson biodiesel chambray gentrify try-hard ethical. Meh ennui Brooklyn PBR&B Vice. Fap 90s mixtape, meh Pitchfork sartorial American Apparel cray tofu small batch tote bag ennui. Intelligentsia Echo Park fingerstache skateboard 3 wolf moon Neutra sartorial Truffaut pop-up. Swag synth lomo High Life messenger bag. Keffiyeh McSweeneys butcher jean shorts. Plaid YOLO twee biodiesel wayfarers. Roof party small batch Thundercats mustache gluten-free. Cred selvage whatever, drinking vinegar Portland vegan Odd Future butcher Austin distillery photo booth. Pinterest cred ethical, pork belly art party hella aesthetic mumblecore dreamcatcher forage brunch Neutra. Banjo chillwave shabby chic fap distillery, gentrify skateboard semiotics Marfa cliche American Apparel next level Truffaut Williamsburg';
  $node->body[$node->language][0]['summary'] = '  ';
  $node->body[$node->language][0]['format'] = 'full_html'; // If field has a format, you need to define it. Here we define a default filtered_html format for a body field
  $node = node_submit($node); // Prepare node for a submit
  node_save($node); // After this call we'll get a nid  
}

if ($ebookstripe_copyright > 0)
{
  array_push($pagesarray, "Copyright");
  $node = new stdClass(); // We create a new node object
  $node->type = "ebookstripe_general"; // Or any other content type you want
  $node->title = "Copyright";
  $node->language = LANGUAGE_NONE; // Or any language code if Locale module is enabled. More on this below *
  $node->uid = 1; // Or any id you wish
  $nodepath = $booktitle . "/copyright";
  $node->path = array('alias' => $nodepath); // Setting a node path
  $node->status = 1;
  $node->promote = 0;
  $node->comment = 1;
  $term_name = $booktitle . "_" . "chapter0";
    $foo = taxonomy_get_term_by_name($term_name);
    foreach($foo as $term) {
        if($term->vid == 1) {
        $node->field_ebookstripe_chapter[$node->language][0]['tid'] = $term->tid;  
        }
    }
  node_object_prepare($node); // Set some default values.
  // Let's add standard body field
  $node->body[$node->language][0]['value'] = 'This is the Copyright ' . $booktitle . '<br>Terry Richardson biodiesel chambray gentrify try-hard ethical. Meh ennui Brooklyn PBR&B Vice. Fap 90s mixtape, meh Pitchfork sartorial American Apparel cray tofu small batch tote bag ennui. Intelligentsia Echo Park fingerstache skateboard 3 wolf moon Neutra sartorial Truffaut pop-up. Swag synth lomo High Life messenger bag. Keffiyeh McSweeneys butcher jean shorts. Plaid YOLO twee biodiesel wayfarers. Roof party small batch Thundercats mustache gluten-free. Cred selvage whatever, drinking vinegar Portland vegan Odd Future butcher Austin distillery photo booth. Pinterest cred ethical, pork belly art party hella aesthetic mumblecore dreamcatcher forage brunch Neutra. Banjo chillwave shabby chic fap distillery, gentrify skateboard semiotics Marfa cliche American Apparel next level Truffaut Williamsburg';
  $node->body[$node->language][0]['summary'] = '  ';
  $node->body[$node->language][0]['format'] = 'full_html'; // If field has a format, you need to define it. Here we define a default filtered_html format for a body field
  $node = node_submit($node); // Prepare node for a submit
  node_save($node); // After this call we'll get a nid  
}

if ($ebookstripe_abouttheauthor > 0)
{
  array_push($pagesarray, "About The Author");
  $node = new stdClass(); // We create a new node object
  $node->type = "ebookstripe_general"; // Or any other content type you want
  $node->title = "About The Author";
  $node->language = LANGUAGE_NONE; // Or any language code if Locale module is enabled. More on this below *
  $node->uid = 1; // Or any id you wish
  $nodepath = $booktitle . "/about_the_author";
  $node->path = array('alias' => $nodepath); // Setting a node path
  $node->status = 1;
  $node->promote = 0;
  $node->comment = 1;
  $term_name = $booktitle . "_" . "chapter0";
    $foo = taxonomy_get_term_by_name($term_name);
    foreach($foo as $term) {
        if($term->vid == 1) {
        $node->field_ebookstripe_chapter[$node->language][0]['tid'] = $term->tid;  
        }
    }
  node_object_prepare($node); // Set some default values.
  // Let's add standard body field
  $node->body[$node->language][0]['value'] = 'This is the About The Author to ' . $booktitle . '<br>Terry Richardson biodiesel chambray gentrify try-hard ethical. Meh ennui Brooklyn PBR&B Vice. Fap 90s mixtape, meh Pitchfork sartorial American Apparel cray tofu small batch tote bag ennui. Intelligentsia Echo Park fingerstache skateboard 3 wolf moon Neutra sartorial Truffaut pop-up. Swag synth lomo High Life messenger bag. Keffiyeh McSweeneys butcher jean shorts. Plaid YOLO twee biodiesel wayfarers. Roof party small batch Thundercats mustache gluten-free. Cred selvage whatever, drinking vinegar Portland vegan Odd Future butcher Austin distillery photo booth. Pinterest cred ethical, pork belly art party hella aesthetic mumblecore dreamcatcher forage brunch Neutra. Banjo chillwave shabby chic fap distillery, gentrify skateboard semiotics Marfa cliche American Apparel next level Truffaut Williamsburg';
  $node->body[$node->language][0]['summary'] = '  ';
  $node->body[$node->language][0]['format'] = 'full_html'; // If field has a format, you need to define it. Here we define a default filtered_html format for a body field
  $node = node_submit($node); // Prepare node for a submit
  node_save($node); // After this call we'll get a nid  
}

if ($ebookstripe_conclusion > 0)
{
  array_push($pagesarray, "Conclusion");
  $node = new stdClass(); // We create a new node object
  $node->type = "ebookstripe_general"; // Or any other content type you want
  $node->title = "Conclusion";
  $node->language = LANGUAGE_NONE; // Or any language code if Locale module is enabled. More on this below *
  $node->uid = 1; // Or any id you wish
  $nodepath = $booktitle . "/conclusion";
  $node->path = array('alias' => $nodepath); // Setting a node path
  $node->status = 1;
  $node->promote = 0;
  $node->comment = 1;
  $term_name = $booktitle . "_" . "chapter0";
    $foo = taxonomy_get_term_by_name($term_name);
    foreach($foo as $term) {
        if($term->vid == 1) {
        $node->field_ebookstripe_chapter[$node->language][0]['tid'] = $term->tid;  
        }
    }
  node_object_prepare($node); // Set some default values.
  // Let's add standard body field
  $node->body[$node->language][0]['value'] = 'This is the Conclusion to ' . $booktitle . '<br>Terry Richardson biodiesel chambray gentrify try-hard ethical. Meh ennui Brooklyn PBR&B Vice. Fap 90s mixtape, meh Pitchfork sartorial American Apparel cray tofu small batch tote bag ennui. Intelligentsia Echo Park fingerstache skateboard 3 wolf moon Neutra sartorial Truffaut pop-up. Swag synth lomo High Life messenger bag. Keffiyeh McSweeneys butcher jean shorts. Plaid YOLO twee biodiesel wayfarers. Roof party small batch Thundercats mustache gluten-free. Cred selvage whatever, drinking vinegar Portland vegan Odd Future butcher Austin distillery photo booth. Pinterest cred ethical, pork belly art party hella aesthetic mumblecore dreamcatcher forage brunch Neutra. Banjo chillwave shabby chic fap distillery, gentrify skateboard semiotics Marfa cliche American Apparel next level Truffaut Williamsburg';
  $node->body[$node->language][0]['summary'] = '  ';
  $node->body[$node->language][0]['format'] = 'full_html'; // If field has a format, you need to define it. Here we define a default filtered_html format for a body field
  $node = node_submit($node); // Prepare node for a submit
  node_save($node); // After this call we'll get a nid  
}

// if ($ebookstripe_tableofcontents > 0)
// {
  $thehtml ="";
  $node = new stdClass(); // We create a new node object
  $node->type = "ebookstripe_general"; // Or any other content type you want
  $node->title = "Table of Contents";
  $node->language = LANGUAGE_NONE; // Or any language code if Locale module is enabled. More on this below *
  $node->uid = 1; // Or any id you wish
  $nodepath = $booktitle . "/table_of_contents";
  $node->path = array('alias' => $nodepath); // Setting a node path
  $node->status = 1;
  $node->promote = 0;
  $node->comment = 1;
  $term_name = $booktitle . "_" . "chapter0";
    $foo = taxonomy_get_term_by_name($term_name);
    foreach($foo as $term) {
        if($term->vid == 1) {
        $node->field_ebookstripe_chapter[$node->language][0]['tid'] = $term->tid;  
        }
    }
  node_object_prepare($node); // Set some default values.
  // Let's add standard body field
  // pagesarray has spaces
    foreach($pagesarray as $key => $value){
    $valuenospaces = strtr ($value, array (' ' => '_')); 
    $thehtml = $thehtml . "<a href='/$booktitle/$valuenospaces'>$value</a><br />";
    }

    for ($x=1; $x<=$chapters; $x++)
    {
    $chaptername = "Chapter " . $x;
    $thehtml = $thehtml . "<a href='/book/$booktitle/chapter/$x'>$chaptername</a><br />";
    }

    $node->body[$node->language][0]['format'] = 'full_html';
    $node->body[$node->language][0]['value'] = $thehtml;
    $node->body[$node->language][0]['summary'] = $thehtml;
$node = node_submit($node); // Prepare node for a submit
node_save($node); // After this call we'll get a nid  
// }



for ($x=1; $x<=$chapters; $x++)
{
//endings
  $chaptername = "ebookstripe_chapter" . $x . "end";
  $chaptername2 = "chapter" . $x;
  
    $stuff = array('#type' => "$chaptername");
    $intersection = array_intersect($stuff, node_type_get_names());
    if (!$intersection)
    {  
  $ending = array(
    'type' => $chaptername,
    'name' => t($chaptername),
    'base' => 'node_content',
    'description' => t('Only members of role type: chapter' . $x . " can view this content"),
    'body_label' => t('Content'),
  );
// Set defaults.
  $content_type = node_type_set_defaults($ending);
  node_add_body_field($content_type);
  node_type_save($content_type);
  $instance = array(
  'field_name' => 'field_ebookstripe_chapter',
  'entity_type' => 'node',
  'bundle' => $chaptername,
  'label' => t('Chapter'),
  'description' => t('Which chapter does this content belong to?'),
  'required' => true,
  'display' => array(
    'teaser' => array(                           // Teaser display mode
      'label' => 'hidden',                       // Position of the label, ie. 'inline', 'above' or 'hidden'
    ),
    'default' => array(
      'label' => 'hidden',
    ),
    'teaser' => array(                           // Teaser display mode
      'type' => 'hidden',                       // Position of the label, ie. 'inline', 'above' or 'hidden'
    ),
    'default' => array(
      'type' => 'hidden',
    ),
  ),
  'widget' => array(
    'type' => 'options_select',
    'weight' => 3
    )
  );
field_create_instance($instance);
    }
  
if($images > 0)
{  
for ($y=1; $y<=$images; $y++)
{
// if images
  $node = new stdClass(); // We create a new node object
  $node->type = "ebookstripe_illustration"; // Or any other content type you want
  $node->title = "Chapter - $x - $y";
  $node->language = LANGUAGE_NONE; // Or any language code if Locale module is enabled. More on this below *
  $node->uid = 1; // Or any id you wish
  $node->status = 1;
  $node->promote = 0;
  $node->comment = 1;
  node_object_prepare($node); // Set some default values.
  
  $term_name = $booktitle . "_" . "chapter" . $x;
  $foo = taxonomy_get_term_by_name($term_name);
  foreach($foo as $term) {
  if($term->vid == 1) {
  $node->field_ebookstripe_chapter[$node->language][0]['tid'] = $term->tid;
    }
  }
  $node->body[$node->language][0]['value'] = 'Place picture here for an illustration for your chapter';
  $node->body[$node->language][0]['summary'] = 'Place picture here for an illustration for your chapter';
  $node->body[$node->language][0]['format'] = 'filtered_html'; // If field has a format, you need to define it. Here we define a default filtered_html format for a body field
  $node = node_submit($node); // Prepare node for a submit
  node_save($node); // After this call we'll get a nid  
}
}

}  



for ($x=1; $x<=$chapters; $x++)
{
// make the beginning nodes
  $node = new stdClass(); // We create a new node object
  $node->type = "ebookstripe_beginning"; // Or any other content type you want
  $node->title = "Chapter $x beginning";
  $node->language = LANGUAGE_NONE; // Or any language code if Locale module is enabled. More on this below *
  $node->uid = 1; // Or any id you wish
  $node->status = 1;
  $node->promote = 0;
  $node->comment = 1;
  // $node->path = array('alias' => 'conclusion'); // Setting a node path
  node_object_prepare($node); // Set some default values.
  
  $term_name = $booktitle . "_" . "chapter" . $x;
  $foo = taxonomy_get_term_by_name($term_name);
foreach($foo as $term) {
    if($term->vid == 1) {
  $node->field_ebookstripe_chapter[$node->language][0]['tid'] = $term->tid;
    }
}
  
  $node->body[$node->language][0]['value'] = "Chapter $x beginning<br>Terry Richardson biodiesel chambray gentrify try-hard ethical. Meh ennui Brooklyn PBR&B Vice. Fap 90s mixtape, meh Pitchfork sartorial American Apparel cray tofu small batch tote bag ennui. Intelligentsia Echo Park fingerstache skateboard 3 wolf moon Neutra sartorial Truffaut pop-up. Swag synth lomo High Life messenger bag. Keffiyeh McSweeneys butcher jean shorts. Plaid YOLO twee biodiesel wayfarers. Roof party small batch Thundercats mustache gluten-free. Cred selvage whatever, drinking vinegar Portland vegan Odd Future butcher Austin distillery photo booth. Pinterest cred ethical, pork belly art party hella aesthetic mumblecore dreamcatcher forage brunch Neutra. Banjo chillwave shabby chic fap distillery, gentrify skateboard semiotics Marfa cliche American Apparel next level Truffaut Williamsburg";
  $node->body[$node->language][0]['summary'] = "Chapter $x beginning<br>Terry Richardson biodiesel chambray gentrify try-hard ethical. Meh ennui Brooklyn PBR&B Vice. Fap 90s mixtape, meh Pitchfork sartorial American Apparel cray tofu small batch tote bag ennui. Intelligentsia Echo Park fingerstache skateboard 3 wolf moon Neutra sartorial Truffaut pop-up. Swag synth lomo High Life messenger bag. Keffiyeh McSweeneys butcher jean shorts. Plaid YOLO twee biodiesel wayfarers. Roof party small batch Thundercats mustache gluten-free. Cred selvage whatever, drinking vinegar Portland vegan Odd Future butcher Austin distillery photo booth. Pinterest cred ethical, pork belly art party hella aesthetic mumblecore dreamcatcher forage brunch Neutra. Banjo chillwave shabby chic fap distillery, gentrify skateboard semiotics Marfa cliche American Apparel next level Truffaut Williamsburg";
  $node->body[$node->language][0]['format'] = 'filtered_html'; // If field has a format, you need to define it. Here we define a default filtered_html format for a body field
  $node = node_submit($node); // Prepare node for a submit
  node_save($node); // After this call we'll get a nid
  
// make the ebookstripenodes
  $node = new stdClass(); // We create a new node object
  $node->type = "ebookstripe_button"; // Or any other content type you want
  $node->title = "Chapter $x More...";
  $node->language = LANGUAGE_NONE; // Or any language code if Locale module is enabled. More on this below *
  $node->uid = 1; // Or any id you wish
  $node->status = 1;
  $node->promote = 0;
  $node->comment = 1;
  // $node->path = array('alias' => 'conclusion'); // Setting a node path
  node_object_prepare($node); // Set some default values.
  
  
  $term_name = $booktitle . "_" . "chapter" . $x;
  $foo = taxonomy_get_term_by_name($term_name);
foreach($foo as $term) {
    if($term->vid == 1) {
  $node->field_ebookstripe_chapter[$node->language][0]['tid'] = $term->tid;  
    }                                           // book/Hello_Dolly/chapter/2 + buynow/chapter2
}                                               // buynow/chapter2
  $price = variable_get('ebookstripe_price'); // book/Moby_Dick/chapter/buynow/Moby_Dick_chapter1
  if ($price) {
  $node->body[$node->language][0]['value'] = "<button onclick=\"window.location.href='buynow/" . $booktitle . "_chapter" . $x . "';\">Buy Full Chapter for $" . $price . "</button>";
  }
  else
  {
$node->body[$node->language][0]['value'] = "<button onclick=\"window.location.href='buynow/" . $booktitle . "_chapter" . $x . "';\">Buy Full Chapter</button>";
  }
  
  if ($price) {
  $node->body[$node->language][0]['summary'] = "<button onclick=\"window.location.href='buynow/" . $booktitle . "_chapter" . $x . "';\">Buy Full Chapter for $" . $price . "</button>";
  }
  else
  {
$node->body[$node->language][0]['summary'] = "<button onclick=\"window.location.href='buynow/" . $booktitle . "_chapter" . $x . "';\">Buy Full Chapter</button>";
  }
  $node->body[$node->language][0]['format'] = 'full_html'; // If field has a format, you need to define it. Here we define a default filtered_html format for a body field
  $node = node_submit($node); // Prepare node for a submit
  node_save($node); // After this call we'll get a nid  
  
// make the ending nodes
  $node = new stdClass(); // We create a new node object
  $node->type = "ebookstripe_chapter" . $x . "end"; // Or any other content type you want
  $node->title = "Chapter $x ending";
  $node->language = LANGUAGE_NONE; // Or any language code if Locale module is enabled. More on this below *
  $node->uid = 1; // Or any id you wish
  $node->status = 1;
  $node->promote = 0;
  $node->comment = 1;
  // $node->path = array('alias' => 'conclusion'); // Setting a node path
  $term_name = $booktitle . "_" . "chapter" . $x;
  $foo = taxonomy_get_term_by_name($term_name);
foreach($foo as $term) {
    if($term->vid == 1) {
  $node->field_ebookstripe_chapter[$node->language][0]['tid'] = $term->tid;
    }
}
  $node->body[$node->language][0]['value'] = "Chapter $x ending<br>Turn-key integrated synergistic impactful rich bleeding-edge robust plug-and-play methodologies 24/365 strategize optimize. Strategic; web-readiness repurpose, portals aggregate; frictionless syndicate widgets. Synergize vortals next-generation engage value deploy. Mashups long-tail peer-to-peer mesh content, front-end world-class, envisioneer capture; brand frictionless, synergize rich cultivate synergistic paradigms, redefine wikis holistic granular niches integrate.Aggregate seamless, harness webservices strategize, web-enabled; remix recontextualize bandwidth vortals e-enable, one-to-one; social eyeballs. Tag open-source relationships channels real-time engineer share blogospheres exploit bleeding-edge e-commerce 24/7 B2B widgets methodologies networkeffects harness integrated mission-critical.";
  $node->body[$node->language][0]['summary'] = "  ";
  $node->body[$node->language][0]['format'] = 'filtered_html'; // If field has a format, you need to define it. Here we define a default filtered_html format for a body field
  $node = node_submit($node); // Prepare node for a submit
  node_save($node); // After this call we'll get a nid
}
drupal_goto();
}

function _ebookstripe_form_notnumeric_ajax_validate($form, &$form_state) {
  // Validate as text, not as number.
  if (is_numeric($form_state['values']['booktitle'])) {
    $error = '<div id="booktitle-error" style="color:#ff0000;">Book titles should not be numbers.</div>';
    return $error;
  }
  else {
    $noerror = '<div id="booktitle-error"> </div>';
    return $noerror;
  }
}

function _ebookstripe_form_numeric1_ajax_validate($form, &$form_state) {
  // Validate as number, not as text.
  if (!is_numeric($form_state['values']['chapters'])) {
    $error = '<div id="chapters-error" style="color:#ff0000;">This field should be a number.</div>';
    return $error;
  }
  else {
    $noerror = '<div id="chapters-error"> </div>';
    return $noerror;
  }
}

function _ebookstripe_form_numeric2_ajax_validate($form, &$form_state) {
  // Validate as number, not as text.
  if (!is_numeric($form_state['values']['images'])) {
    $error = '<div id="images-error" style="color:#ff0000;">This field should be a number.</div>';
    return $error;
  }
  else {
    $noerror = '<div id="images-error"> </div>';
    return $noerror;
  }
}


function ebookstripe_creator_form_validate($form, &$form_state) {
  if (is_numeric($form_state['values']['booktitle']) || strlen($form_state['values']['booktitle']) > 75) {
form_set_error('creator][booktitle', t('Book titles should not be numbers and less than 75 characters.'));
  }
  elseif (!is_numeric($form_state['values']['chapters']) || $form_state['values']['chapters'] < 0  || $form_state['values']['chapters'] > 50)
  {
form_set_error('billing][email', t('How many chapters field should be a positive number below 50 to start.'));
  }
  elseif (!is_numeric($form_state['values']['images']) || $form_state['values']['images'] < 0  || $form_state['values']['images'] > 10)
  {
form_set_error('billing][email', t('How many images field should be a positive number below 10 to start.'));
  }
  else {
    return;
  }
}