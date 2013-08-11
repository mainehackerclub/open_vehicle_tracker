require 'bundler'
Bundler.require

set :server, :thin
connections = []

DataMapper.setup(:default, 'postgres://sinatra:api@localhost/ovt')

class Vehicle
  include DataMapper::Resource
  has n, :telemetries

  property :id, Serial, :key => true
  property :type, String
end

class Telemetry
  include DataMapper::Resource
  belongs_to :vehicle

  property :id, Serial, :key => true
  property :lat, Float
  property :lon, Float
  property :time, DateTime
end

DataMapper.finalize
DataMapper.auto_upgrade!

post '/vehicles' do
  content_type :json
  @vehicles = Vehicle.new(params)

  if @vehicles.save
    @vehicles.to_json
  else
    halt 500
  end
end

get '/vehicles' do
  content_type :json
  @vehicles = Vehicle.all

  @vehicles.to_json
end

post '/telemetries' do
  content_type :json
  @telemetry = Telemetry.new(params)

  if @telemetry.save
    @telemetry.to_json
  else
    halt 500
  end
end

get '/vehicle/telemetries/:id' do
  content_type :json
  @telemetries = Telemetry.all(:vehicle_id => params[:id])

  @telemetries.to_json
end

#get '/stream/telemetries/vehicle/:id' do
  #content_type :json

  #stream do |out|
    
  #end
#end
