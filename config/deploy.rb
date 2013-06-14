
set :ssh_options, {:forward_agent => true}
set :use_sudo, false
set :application, 'open_vehicle_tracker'
set :domain, '66.172.10.186'
set :repository,  'git@github.com:mainehackerclub/open_vehicle_tracker.git'
set :scm, 'git'
set :deploy_to,   '/www/ovt'
set :user, 'deployer'
set :branch, 'master'
set :deploy_via, :remote_cache

role :web, domain  # Your HTTP server, nginx/etc
role :app, domain  # This may be the same as your `Web` server

# if you're still using the script/reaper helper you will need
# these http://github.com/rails/irs_process_scripts

# If you are using Passenger mod_rails uncomment this:
# namespace :deploy do
#   task :start do ; end
#   task :stop do ; end
#   task :restart, :roles => :app, :except => { :no_release => true } do
#     run "#{try_sudo} touch #{File.join(current_path,'tmp','restart.txt')}"
#   end
# end
