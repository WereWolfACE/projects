/*
 *  TimeMachine.h
 *  Engine
 *
 *  Created by vasiliym on 02.12.09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _TIMEMACHINE_H_
#define _TIMEMACHINE_H_

#include "Core/MessageManager.h"
#include "RefCounter.h"
#include "ThreadSupport.h"

#include <boost/intrusive_ptr.hpp>

namespace Core {

	/// Базовый класс отложенного действия
	class JobBase : public RefCounter {
	public:
		virtual void Do() = 0;
	};

	typedef boost::intrusive_ptr<JobBase> JobBasePtr;
	
	/// Класс для отложенного выполнения некоторых действий
	class TimeMachine {
	public:
		typedef void(*Callback)(void *);

		TimeMachine();
		~TimeMachine();
		
		/// Выполнить абстрактную работу через заданное кол-во миллисекунд
		/// Возвращает уникальный идентификатор задания
		int doJob(JobBase* job, int msec);
		/// Выполнить lua строку через заданное кол-во миллисекунд
		/// Возвращает уникальный идентификатор задания
		int doString(const std::string& command, int msec);
		/// Отправить сообщение через заданное кол-во миллисекунд
		/// Возвращает уникальный идентификатор задания
		int doMessage(const Message& message, int msec);
		/// Вызвать заданную функцию через заданное кол-во миллисекунд
		/// Возвращает уникальный идентификатор задания
		int doCallback(Callback func, void* userData, int msec);
		/// Отменить выполнение указанного задания
		/// Возвращает 0 если задание отменено, -1 если такого задания нет, 1 если задание уже было выполнено
		int cancelJob(int job);
		/// Возвращает 0 если задание есть в очереди, -1 если такого задания нет, 1 если задание уже было выполнено
		int getState(int job);
		/// 
		void Update(float dt);
		/// 
		void Clear();
		///
		bool IsFinished() const;
		///
		void Pause(bool pause);

	private:
		enum {
			UNKNOWN_TYPE = 0, COMMAND_TYPE, MESSAGE_TYPE, CALLBACK_TYPE, JOB_TYPE
		};
		struct Job {
			JobBasePtr job;
			std::string command;
			Message message;
			Callback func;
			void* userData;
			int type;
			float time;
			Job();
			Job(JobBase* job, int msec);
			Job(const std::string& c, int msec);
			Job(const Message& m, int msec);
			Job(Callback func, void* userData, int msec);
			void doJob();
		};

		typedef std::map<int, Job> JobMap;

		mutable MUTEX_TYPE _mutex;
		JobMap _jobs;
		std::vector<Job> _toBeDone;
		std::vector<int> _finished;
		int _nextId;
		bool _pause;
	};
	
	extern TimeMachine timeMachine;
}

#endif // _TIMEMACHINE_H_