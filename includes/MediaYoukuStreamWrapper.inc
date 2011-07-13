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
  protected $base_url = 'http://v.youku.com/v_show/'; function getTarget($f) {
    return FALSE;
  }

  static
  function getMimeType($uri, $mapping = NULL) {
    return 'video/youku';
  }

  function fetch_page($url) {
    $cache_id = 'media_youku_' . $url;
    $cache = cache_get($cache_id);
    if ($cache && $cache->data) {
      $html = $cache->data;
    }
    else {
      $result = drupal_http_request($url);
      $code   = floor($result->code / 100) * 100;
      $types  = array('text/html');
      if ($result->data && $code != 400 && $code != 500 && in_array($result->Content - Type, $types)) {
        cache_set($cache_id, $result->data);
        $html = $result->data;
      }
      else {
        //if we are unsuccessful then log a message in watchdog
        watchdog('media_youku', 'The html ' . $url . ' could not be retrieved');
        return FALSE;
      }
    }
    return $html;
  }

  function getOriginalThumbnailPath() {
    $parts = $this->get_parameters();
    if ($html = $this->fetch_page($this->base_url . '/v_show/id_' . $parts['id'] . '.html')) {
      preg_match_all('@id="download" href="iku:\/\/\|([^"]+)"@', $html, $matches);
      if (isset($matches[1][0])) {
        $match = explode('|', $match);
        array_pop($match);
        $thumb = array_pop($match);
      }
    }
    return $thumb;
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

