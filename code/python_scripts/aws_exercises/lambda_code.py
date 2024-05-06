def lambda_handler(event, context):
    # Your code logic here
    name = event.get('name', 'Anonymous')
    greeting = 'Hello, ' + name + '!'
    return {
        'statusCode': 200,
        'body': greeting
    }
