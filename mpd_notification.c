#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libnotify/notify.h>
#include <mpd/client.h>

#define SLEEP_TIME		5
#define NOTIFICATION_FORMAT	"<b>%s</b>\n<i>%s</i>\n%s"

/**
** Return an allocated string (caller has to free it) containing the
** informations needed to be printed.
*/
char *
format_notification(char *title, char *album, char *artist)
{
  const char unavailable[] = "N/A";
  char *str;
  unsigned int str_length = strlen(NOTIFICATION_FORMAT);

  if (title != NULL)
  {
    title = g_markup_escape_text(title, -1);
    str_length += strlen(title);
  }
  else
    str_length += strlen(unavailable);

  if (album != NULL)
  {
    album = g_markup_escape_text(album, -1);
    str_length += strlen(album);
  }
  else
    str_length += strlen(unavailable);

  if (artist != NULL)
  {
    artist = g_markup_escape_text(artist, -1);
    str_length += strlen(artist);
  }
  else
    str_length += strlen(unavailable);

  str = malloc(sizeof (char) * str_length);
  if (str != NULL)
  {
    if (snprintf(str, str_length, NOTIFICATION_FORMAT,
	  title ? title : unavailable, album ? album : unavailable,
	  artist ? artist : unavailable) == -1)
      str[str_length - 1] = '\0';
  }

  free(title);
  free(album);
  free(artist);

  return str;
}

void
infinite_loop(struct mpd_connection *conn)
{
  char *notification;
  struct mpd_status *status;
  NotifyNotification *netlink = NULL;
  time_t start_time = time(NULL);

  while (mpd_run_idle_mask(conn, 0xff))
  {
    mpd_command_list_begin(conn, true);
    mpd_send_status(conn);
    mpd_send_current_song(conn);
    mpd_command_list_end(conn);

    if ((status = mpd_recv_status(conn)) == NULL)
    {
      fprintf(stderr, "Cannot connect to MPD. Retrying...");
      continue;
    }
    if (mpd_status_get_state(status) == MPD_STATE_STOP)
      notification = strdup("Stopped playback");
    else if (mpd_status_get_state(status) == MPD_STATE_PLAY)
    {
      struct mpd_song *song;
      char *title, *album, *artist;

      mpd_response_next(conn);
      song = mpd_recv_song(conn);

      title = (char *)mpd_song_get_tag(song, MPD_TAG_TITLE, 0);
      album = (char *)mpd_song_get_tag(song, MPD_TAG_ALBUM, 0);
      artist = (char *)mpd_song_get_tag(song, MPD_TAG_ARTIST, 0);

      notification = format_notification(title, album, artist);
      mpd_song_free(song);
    }
    else if (mpd_status_get_state(status) == MPD_STATE_PAUSE)
      notification = strdup("Paused playback");
    else
      notification = strdup("tagadatugrut");

    if (netlink != NULL && time(NULL) - start_time >= SLEEP_TIME)
    {
      notify_notification_close(netlink, NULL);
      netlink = NULL;
    }

    while (netlink == NULL)
    {
      netlink = notify_notification_new("MPD Notification", NULL, "sound");
      start_time = time(NULL);
    }

    notify_notification_update(netlink, "MPD", notification, "sound");

    notify_notification_show(netlink, NULL);
    mpd_response_finish(conn);
    free(notification);
  }
}

int
main(int argc, char *argv[])
{
  struct mpd_connection *conn = mpd_connection_new(NULL, 0, 0);

  if (mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS)
  {
    fprintf(stderr,"%s: %s\n", argv[0], mpd_connection_get_error_message(conn));
    return 1;
  }

  if (notify_init("MPD_Notification") == 0)
  {
    fprintf(stderr, "%s: Cannot initialize libnotify.\n", argv[0]);
    return 1;
  }

  infinite_loop(conn);

  mpd_connection_free(conn);
  notify_uninit();

  return 0;
}
