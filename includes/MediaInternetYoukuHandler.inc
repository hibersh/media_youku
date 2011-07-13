<?php

/**
 * Implementation of MediaInternetBaseHandler.
 *
 * @see hook_media_internet_providers().
 */
class MediaInternetYoukuHandler extends MediaInternetBaseHandler {
  public function parse($embedCode) {
    $patterns = array(
      '@v\.youku\.com/v_show/id_([^"\&/]+)\.html@i',
      '@player\.youku\.com/player\.php/sid/([^"\&/]+)@i',
    );
    foreach ($patterns as $pattern) {
      preg_match($pattern, $embedCode, $matches);
      if (isset($matches[1]
        )) {
        return file_stream_wrapper_uri_normalize('youku://id/' . $matches[1]);
      }
    }
  }

  public function claim($embedCode) {
    if ($this->parse($embedCode)) {
      return TRUE;
    }
  }

  public function validate() {
    // @todo Media module currently fails when two files try to have the same
    //   URI, so catch that in the validation step. Some day, it would be nice
    //   to allow it, however. See http://drupal.org/node/952422.
    $uri = $this->parse($this->embedCode);
    $existing_files = file_load_multiple(array(), array('uri' => $uri));
    if (count($existing_files)) {
      thrownewMediaInternetValidationException(t('You have entered a URL for a video that is already in your library.'));
    }
  }

  public function save() {
    $file = $this->getFileObject();
    file_save($file);
    return $file;
  }

  public function getFileObject() {
    $uri = $this->parse($this->embedCode);
    //@todo: this is terribly broken in some ways because the function is really
    // made for local files which are 'real'
    return file_uri_to_object($uri);
  }
}

