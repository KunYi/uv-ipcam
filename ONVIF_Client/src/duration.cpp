#include "soapH.h"

void soap_default_xsd__duration(struct soap *soap, LONG64 *a)
{ (void)soap; /* appease -Wall -Werror */
  *a = 0;
}

const char *soap_xsd__duration2s(struct soap *soap, LONG64 a)
{ LONG64 d;
  int k, h, m, s, f;
  if (a < 0)
  { strcpy(soap->tmpbuf, "-P");
    k = 2;
    a = -a;
  }
  else
  { strcpy(soap->tmpbuf, "P");
    k = 1;
  }
  f = a % 1000;
  a /= 1000;
  s = a % 60;
  a /= 60;
  m = a % 60;
  a /= 60;
  h = a % 24;
  d = a / 24;
  if (d)
    sprintf(soap->tmpbuf + k, SOAP_LONG_FORMAT "D", d);
  if (h || m || s || f)
  { if (d)
      k = strlen(soap->tmpbuf);
    if (f)
      sprintf(soap->tmpbuf + k, "T%dH%dM%d.%03dS", h, m, s, f);
    else
      sprintf(soap->tmpbuf + k, "T%dH%dM%dS", h, m, s);
  }
  else if (!d)
    strcpy(soap->tmpbuf + k, "T0S");
  return soap->tmpbuf;
}

int soap_out_xsd__duration(struct soap *soap, const char *tag, int id, const LONG64 *a, const char *type)
{ if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_xsd__duration), type)
   || soap_string_out(soap, soap_xsd__duration2s(soap, *a), 0))
    return soap->error;
  return soap_element_end_out(soap, tag);
}

int soap_s2xsd__duration(struct soap *soap, const char *s, LONG64 *a)
{ LONG64 sign = 1, Y = 0, M = 0, D = 0, H = 0, N = 0, S = 0;
  float f = 0;
  *a = 0;
  if (s)
  { if (*s == '-')
    { sign = -1;
      s++;
    }
    if (*s++ != 'P')
      return soap->error = SOAP_TYPE;
    /* date part */
    while (s && *s)
    { LONG64 n;
      char k;
      if (*s == 'T')
      { s++;
	break;
      }
      if (sscanf(s, SOAP_LONG_FORMAT "%c", &n, &k) != 2)
	return soap->error = SOAP_TYPE;
      s = strchr(s, k);
      if (!s)
	return soap->error = SOAP_TYPE;
      switch (k)
      { case 'Y':
	  Y = n;
	  break;
	case 'M':
	  M = n;
	  break;
	case 'D':
	  D = n;
	  break;
	default:
	  return soap->error = SOAP_TYPE;
      }
      s++;
    }
    /* time part */
    while (s && *s)
    { LONG64 n;
      char k;
      if (sscanf(s, SOAP_LONG_FORMAT "%c", &n, &k) != 2)
	return soap->error = SOAP_TYPE;
      s = strchr(s, k);
      if (!s)
	return soap->error = SOAP_TYPE;
      switch (k)
      { case 'H':
	  H = n;
	  break;
	case 'M':
	  N = n;
	  break;
	case '.':
	  S = n;
	  if (sscanf(s, "%g", &f) != 1)
	    return soap->error = SOAP_TYPE;
	  s = NULL;
	  continue;
	case 'S':
	  S = n;
	  break;
	default:
	  return soap->error = SOAP_TYPE;
      }
      s++;
    }
    /* convert Y-M-D H:N:S.f to signed long long int */
    *a = sign * ((((((((((((Y * 12) + M) * 30) + D) * 24) + H) * 60) + N) * 60) + S) * 1000) + (long)(1000.0 * f));
  }
  return soap->error;
}

LONG64 *soap_in_xsd__duration(struct soap *soap, const char *tag, LONG64 *a, const char *type)
{ if (soap_element_begin_in(soap, tag, 0, NULL))
    return NULL;
  if (*soap->type
   && soap_match_tag(soap, soap->type, type)
   && soap_match_tag(soap, soap->type, ":duration"))
  { soap->error = SOAP_TYPE;
    soap_revert(soap);
    return NULL;
  }
  a = (LONG64*)soap_id_enter(soap, soap->id, a, SOAP_TYPE_xsd__duration, sizeof(LONG64), 0, NULL, NULL, NULL);
  if (*soap->href)
    a = (LONG64*)soap_id_forward(soap, soap->href, a, 0, SOAP_TYPE_xsd__duration, 0, sizeof(LONG64), 0, NULL);
  else if (a)
  { if (soap_s2xsd__duration(soap, soap_value(soap), a))
      return NULL;
  }
  if (soap->body && soap_element_end_in(soap, tag))
    return NULL;
  return a;
}
