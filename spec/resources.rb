require 'openssl'

module Resources

  CERTIFICATE = OpenSSL::X509::Certificate.new <<-_EOF_
-----BEGIN CERTIFICATE-----
MIIDPTCCAiWgAwIBAgIBAzANBgkqhkiG9w0BAQQFADA9MRMwEQYKCZImiZPyLGQB
GRYDb3JnMRkwFwYKCZImiZPyLGQBGRYJcnVieS1sYW5nMQswCQYDVQQDDAJDQTAe
Fw0xMTEwMjkxNTM2MzJaFw0xMTEwMjkxNjA2MzJaMD4xEzARBgoJkiaJk/IsZAEZ
FgNvcmcxGTAXBgoJkiaJk/IsZAEZFglydWJ5LWxhbmcxDDAKBgNVBAMMA0VFMjCB
nzANBgkqhkiG9w0BAQEFAAOBjQAwgYkCgYEAy8LEsNRApz7U/j5DoB4XBgO9Z8At
v5y/OVQRp0ag8Tqo1YewsWijxEWB7JOATwpBN267U4T1nPZIxxEEO7n/WNa2ws9J
Wsjah8ssEBFSxZqdXKSLf0N4Hi7/GQ/aYoaMCiQ8jA4jegK2FJmXM71uPe+jFN/p
eeBOpRfyXxRFOYcCAwEAAaOByjCBxzAOBgNVHQ8BAf8EBAMCBeAwHQYDVR0OBBYE
FNH++fv4rhvBYMv6A+JZbdhzCJITME8GA1UdIwRIMEahQaQ/MD0xEzARBgoJkiaJ
k/IsZAEZFgNvcmcxGTAXBgoJkiaJk/IsZAEZFglydWJ5LWxhbmcxCzAJBgNVBAMM
AkNBggEBMCcGA1UdJQQgMB4GCCsGAQUFBwMCBggrBgEFBQcDBAYIKwYBBQUHAwMw
HAYDVR0RBBUwE4ERZWUyQHJ1YnktbGFuZy5vcmcwDQYJKoZIhvcNAQEEBQADggEB
ABLJKAS9nXFI3idP3dXXs8n4gLBrWHyaacklwnj8gvVoNtYQIICXODY/zQLtzfjx
SQxlrOUtn0uYTzJUNc+NLfc8EzcVDhmSWhq+msCFEyhL5+kBceFj2ZI8f9OuQ87m
zy1yqE1jxP1fsGDZ3Cqny86jJvwRzPW6NkPtkFzS6ZYe09DXDZnUuZP8kVpn4Pit
vHmj/XVKl4LGxlrebReqIn+m2K46uxUkQEDhk7R16SAl6N92W7vI/lXr98aVdfF4
Ozmrzopvt+XbjtvJztDNhpG0G/celhgAOzNx/eq3wl89qHIa+9vvVbFZ0BdYUHai
z+64/9KqwBAeA66J6k7l7C0=
-----END CERTIFICATE-----
_EOF_
   
  class << self
    def certificate
      CERTIFICATE.to_der
    end

    def certificate_io
      File.new(File.expand_path('res/certificate.cer', File.dirname(__FILE__)))
    end

    def bytes_to_io(str)
      raw = [str.join('')].pack('H*')
      StringIO.new raw  
    end

    def bytes(str)
      [str.join('')].pack('H*')
    end
  end

end

