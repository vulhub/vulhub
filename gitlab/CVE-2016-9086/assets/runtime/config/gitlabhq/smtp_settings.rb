# To enable smtp email delivery for your GitLab instance do the following:
# 1. Rename this file to smtp_settings.rb
# 2. Edit settings inside this file
# 3. Restart GitLab instance
#
# For full list of options and their values see http://api.rubyonrails.org/classes/ActionMailer/Base.html
#
# If you change this file in a Merge Request, please also create a Merge Request on https://gitlab.com/gitlab-org/omnibus-gitlab/merge_requests

if Rails.env.production?
  Rails.application.config.action_mailer.delivery_method = :smtp

  ActionMailer::Base.delivery_method = :smtp
  ActionMailer::Base.smtp_settings = {
    address: "{{SMTP_HOST}}",
    port: {{SMTP_PORT}},
    user_name: "{{SMTP_USER}}",
    password: "{{SMTP_PASS}}",
    domain: "{{SMTP_DOMAIN}}",
    authentication: "{{SMTP_AUTHENTICATION}}",
    enable_starttls_auto: {{SMTP_STARTTLS}},
    openssl_verify_mode: "{{SMTP_OPENSSL_VERIFY_MODE}}",
    ca_path: "{{SMTP_CA_PATH}}",
    ca_file: "{{SMTP_CA_FILE}}",
    tls: {{SMTP_TLS}}
  }
end
