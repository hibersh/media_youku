<?php

/**
 *  @file
 *  Create a Youku Stream Wrapper class for the Media/Resource module.
 */

/**
 *  Create an instance like this:
 *  $youtube = new ResourceYoukuStreamWrapper('youku://id/[video-code]');
 */
class MediaYoukuStreamWrapper extends MediaReadOnlyStreamWrapper {
  protected $base_url = 'http://v.youku.com/v_show/';
  
  function getTarget($f) {
    return FALSE;
  }

  static
  function getMimeType($uri, $mapping = NULL) {
    return 'video/youku';
  }

  function getOriginalThumbnailPath() {
    $parts = $this->get_parameters();
    $html = file_get_contents($this->base_url . 'id_' . $parts['id'] . '.html');
    preg_match_all("/\+0800\|(.*?)\|\">/i", $html, $img);
    return $img[1][0];
  }

  function getLocalThumbnailPath() {
    $parts = $this->get_parameters();
    $local_path = 'public://media-youku/' . check_plain($parts['id']) . '.jpg';
    if (!file_exists($local_path)) {
      $dirname = drupal_dirname($local_path);
      file_prepare_directory($dirname, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
      @copy($this->getOriginalThumbnailPath(), $local_path);
    }
    return $local_path;
  }
}

